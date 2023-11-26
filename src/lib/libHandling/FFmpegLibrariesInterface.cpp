/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "FFmpegLibrariesInterface.h"

#include <cstdarg>

namespace ffmpeg
{

namespace
{

std::vector<std::string> getPossibleLibraryNames(std::string libraryName, int version)
{
  // The ffmpeg libraries are named using a major version number. E.g: avutil-55.dll on windows.
  // On linux, the libraries may be named differently. On Ubuntu they are named
  // libavutil-ffmpeg.so.55. On arch linux the name is libavutil.so.55. We will try to look for both
  // namings. On MAC os (installed with homebrew), there is a link to the lib named
  // libavutil.54.dylib.

#if defined(_WIN32)
  return {{libraryName + "-" + std::to_string(version)}};
#elif defined(__APPLE__)
  return {{"lib" + libraryName + "." + std::to_string(version) + ".dylib"}};
#else
  return {{"lib" + libraryName + "-ffmpeg.so." + std::to_string(version)},
          {"lib" + libraryName + ".so." + std::to_string(version)}};
#endif

  return {};
}

bool tryLoadLibraryInPath(SharedLibraryLoader         &lib,
                          const std::filesystem::path &absoluteDirectoryPath,
                          const std::string           &libName,
                          const Version               &version,
                          Log                         &log)
{
  log.push_back("Trying to load library " + libName + " in path " + absoluteDirectoryPath.string());

  for (const auto &possibleLibName : getPossibleLibraryNames(libName, version.major))
  {
    const auto absolutePathOrLibName = absoluteDirectoryPath / possibleLibName;
    if (!absoluteDirectoryPath.empty())
    {
      const auto fileStatus = std::filesystem::status(absolutePathOrLibName);

      if (fileStatus.type() == std::filesystem::file_type::not_found)
      {
        log.push_back("Loading using lib name " + possibleLibName + " failed. Can not find file " +
                      absolutePathOrLibName.string());
        continue;
      }
    }

    const auto success = lib.load(absolutePathOrLibName);
    log.push_back("Loading library " + absolutePathOrLibName.string() +
                  (success ? " succeded" : " failed"));
    if (success)
      return true;
  }
  return false;
};

bool checkLibraryVersion(const std::string &libName,
                         unsigned           ffmpegVersionOfLoadedLibrary,
                         const Version     &expectedVersion,
                         Log               &log)
{
  const auto loadedVersion = Version::fromFFmpegVersion(ffmpegVersionOfLoadedLibrary);
  if (loadedVersion != expectedVersion)
  {
    log.push_back("Version of loaded " + libName + " library (" + to_string(loadedVersion) +
                  ") is not the one we are trying to load (" + to_string(expectedVersion) + ")");
    return false;
  }

  log.push_back("Version check for library " + libName + " successfull. Version " +
                to_string(loadedVersion) + ".");
  return true;
}

std::filesystem::path getAbsolutePath(const std::filesystem::path &path)
{
  if (path == "." || path == "./")
    return std::filesystem::current_path();

  return std::filesystem::absolute(path);
}

// These FFmpeg versions are supported. The numbers indicate the major version of
// the following libraries in this order: avformat, avcodec, avutil, swresample
// The versions are sorted from newest to oldest, so that we try to open the newest ones first.
auto SupportedMajorLibraryVersionCombinations = {
    LibraryVersions({Version(60), Version(60), Version(58), Version(4)}),
    LibraryVersions({Version(59), Version(59), Version(57), Version(4)}),
    LibraryVersions({Version(58), Version(58), Version(56), Version(3)}),
    LibraryVersions({Version(57), Version(57), Version(55), Version(2)}),
    LibraryVersions({Version(56), Version(56), Version(54), Version(1)}),
};

} // namespace

ResultAndLog
FFmpegLibrariesInterface::tryLoadFFmpegLibrariesInPath(const std::filesystem::path &path)
{
  Log log;

  std::filesystem::path absoluteDirectoryPath;
  if (!path.empty())
  {
    if (!std::filesystem::exists(path))
    {
      log.push_back("The given path (" + path.string() + ") could not be found");
      return {false, log};
    }

    absoluteDirectoryPath = getAbsolutePath(path);
    log.push_back("Using absolute path " + absoluteDirectoryPath.string());
  }

  for (const auto &libraryVersions : SupportedMajorLibraryVersionCombinations)
  {
    this->unloadAllLibraries();
    log.push_back("Unload libraries");

    if (this->tryLoadLibrariesBindFunctionsAndCheckVersions(
            absoluteDirectoryPath, libraryVersions, log))
    {
      log.push_back(
          "Loading of ffmpeg libraries successfully finished. FFmpeg is ready to be used.");
      return {true, log};
    }
  }

  this->unloadAllLibraries();
  log.push_back("Unload libraries");
  log.push_back(
      "We tried all supported versions in given path. Loading of ffmpeg libraries in path failed.");

  return {false, log};
}

bool FFmpegLibrariesInterface::tryLoadLibrariesBindFunctionsAndCheckVersions(
    const std::filesystem::path &absoluteDirectoryPath,
    const LibraryVersions       &libraryVersions,
    Log                         &log)
{
  // AVUtil

  if (!tryLoadLibraryInPath(
          this->libAvutil, absoluteDirectoryPath, "avutil", libraryVersions.avutil, log))
    return false;

  if (const auto functions = functions::tryBindAVUtilFunctionsFromLibrary(this->libAvutil, log))
    this->avutil = functions.value();
  else
    return false;

  if (!checkLibraryVersion("avUtil", this->avutil.avutil_version(), libraryVersions.avutil, log))
    return false;

  // SWResample

  if (!tryLoadLibraryInPath(this->libSwresample,
                            absoluteDirectoryPath,
                            "swresample",
                            libraryVersions.swresample,
                            log))
    return false;

  if (const auto functions =
          functions::tryBindSwResampleFunctionsFromLibrary(this->libSwresample, log))
    this->swresample = functions.value();
  else
    return false;

  if (!checkLibraryVersion(
          "swresample", this->swresample.swresample_version(), libraryVersions.swresample, log))
    return false;

  // AVCodec

  if (!tryLoadLibraryInPath(
          this->libAvcodec, absoluteDirectoryPath, "avcodec", libraryVersions.avcodec, log))
    return false;

  if (const auto functions = functions::tryBindAVCodecFunctionsFromLibrary(this->libAvcodec, log))
    this->avcodec = functions.value();
  else
    return false;

  if (!checkLibraryVersion(
          "avcodec", this->avcodec.avcodec_version(), libraryVersions.avcodec, log))
    return false;

  // AVFormat

  if (!tryLoadLibraryInPath(
          this->libAvformat, absoluteDirectoryPath, "avformat", libraryVersions.avformat, log))
    return false;

  if (const auto functions = functions::tryBindAVFormatFunctionsFromLibrary(this->libAvformat, log))
    this->avformat = functions.value();
  else
    return false;

  if (!checkLibraryVersion(
          "avformat", this->avformat.avformat_version(), libraryVersions.avformat, log))
    return false;

  // Success

  this->libraryVersions.avutil   = Version::fromFFmpegVersion(this->avutil.avutil_version());
  this->libraryVersions.avcodec  = Version::fromFFmpegVersion(this->avcodec.avcodec_version());
  this->libraryVersions.avformat = Version::fromFFmpegVersion(this->avformat.avformat_version());
  this->libraryVersions.swresample =
      Version::fromFFmpegVersion(this->swresample.swresample_version());

  this->avutil.av_log_set_callback(&FFmpegLibrariesInterface::avLogCallback);

  if (this->libraryVersions.avformat.major < 59)
    this->avformat.av_register_all();

  return true;
}

void FFmpegLibrariesInterface::unloadAllLibraries()
{
  this->libAvutil.unload();
  this->libSwresample.unload();
  this->libAvcodec.unload();
  this->libAvformat.unload();
}

std::vector<LibraryInfo> FFmpegLibrariesInterface::getLibrariesInfo() const
{
  if (!this->libAvutil || !this->libSwresample || !this->libAvcodec || !this->libAvformat)
    return {};

  std::vector<LibraryInfo> infoPerLIbrary;

  auto addLibraryInfo = [&infoPerLIbrary](const char                  *name,
                                          const std::filesystem::path &path,
                                          const unsigned               ffmpegVersion)
  {
    const auto libraryVersion = Version::fromFFmpegVersion(ffmpegVersion);
    const auto version        = to_string(libraryVersion);

    infoPerLIbrary.push_back(LibraryInfo({name, path, version}));
  };

  addLibraryInfo("AVFormat", this->libAvformat.getLibraryPath(), this->avformat.avformat_version());
  addLibraryInfo("AVCodec", this->libAvcodec.getLibraryPath(), this->avcodec.avcodec_version());
  addLibraryInfo("AVUtil", this->libAvutil.getLibraryPath(), this->avutil.avutil_version());
  addLibraryInfo(
      "SwResample", this->libSwresample.getLibraryPath(), this->swresample.swresample_version());

  return infoPerLIbrary;
}

std::string FFmpegLibrariesInterface::logListFFmpeg;

void FFmpegLibrariesInterface::avLogCallback(void *, int level, const char *fmt, va_list vargs)
{
  std::string message;
  va_list     vargs_copy;
  va_copy(vargs_copy, vargs);
  size_t len = vsnprintf(0, 0, fmt, vargs_copy);
  message.resize(len);
  vsnprintf(&message[0], len + 1, fmt, vargs);

  FFmpegLibrariesInterface::logListFFmpeg.append("Level " + std::to_string(level) + message);
}

} // namespace ffmpeg
