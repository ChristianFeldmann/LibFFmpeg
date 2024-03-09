/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "FFmpegLibraries.h"

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
  return {{libraryName + "-" + std::to_string(version) + ".dll"}};
#elif defined(__APPLE__)
  return {{"lib" + libraryName + "." + std::to_string(version) + ".dylib"}};
#else
  return {{"lib" + libraryName + "-ffmpeg.so." + std::to_string(version)},
          {"lib" + libraryName + ".so." + std::to_string(version)}};
#endif

  return {};
}

bool checkLibraryVersion(const std::string &    libName,
                         unsigned               ffmpegVersionOfLoadedLibrary,
                         const Version &        expectedVersion,
                         const LoggingFunction &log)
{
  const auto loadedVersion = Version::fromFFmpegVersion(ffmpegVersionOfLoadedLibrary);
  if (loadedVersion != expectedVersion)
  {
    log(LogLevel::Info,
        "Version of loaded " + libName + " library (" + loadedVersion.toString() +
            ") is not the one we are trying to load (" + expectedVersion.toString() + ")");
    return false;
  }

  log(LogLevel::Debug,
      "Version check for library " + libName + " successfull. Version " + loadedVersion.toString() +
          ".");
  return true;
}

std::filesystem::path getAbsolutePath(const std::filesystem::path &path)
{
  if (path == "." || path == "./")
    return std::filesystem::current_path();

  return std::filesystem::absolute(path);
}

} // namespace

bool FFmpegLibraries::tryLoadFFmpegLibrariesInPath(const std::filesystem::path &path)
{
  std::filesystem::path absoluteDirectoryPath;
  if (!path.empty())
  {
    if (!std::filesystem::exists(path))
    {
      this->log(LogLevel::Error, "The given path (" + path.string() + ") could not be found");
      return false;
    }

    absoluteDirectoryPath = getAbsolutePath(path);
    this->log(LogLevel::Info, "Using absolute path " + absoluteDirectoryPath.string());
  }

  for (const auto &version : SupportedFFmpegVersions)
  {
    this->unloadAllLibraries();
    this->log(LogLevel::Debug, "Unload libraries");

    if (this->tryLoadLibrariesBindFunctionsAndCheckVersions(absoluteDirectoryPath, version))
    {
      this->log(LogLevel::Info,
                "Loading of ffmpeg libraries successfully finished. FFmpeg is ready to be used.");
      return true;
    }
  }

  this->unloadAllLibraries();
  this->log(LogLevel::Debug, "Unload libraries");
  this->log(
      LogLevel::Error,
      "We tried all supported versions in given path. Loading of ffmpeg libraries in path failed.");

  return false;
}

bool FFmpegLibraries::tryLoadLibrariesBindFunctionsAndCheckVersions(
    const std::filesystem::path &absoluteDirectoryPath, const LibraryVersions &libraryVersions)
{
  // AVUtil

  if (!this->tryLoadLibraryInPath(
          this->libAvutil, absoluteDirectoryPath, "avutil", libraryVersions.avutil))
    return false;

  if (const auto functions = internal::functions::tryBindAVUtilFunctionsFromLibrary(
          this->libAvutil, this->loggingFunction))
    this->avutil = functions.value();
  else
    return false;

  if (!checkLibraryVersion(
          "avUtil", this->avutil.avutil_version(), libraryVersions.avutil, this->loggingFunction))
    return false;

  // SWResample

  if (!this->tryLoadLibraryInPath(
          this->libSwresample, absoluteDirectoryPath, "swresample", libraryVersions.swresample))
    return false;

  if (const auto functions = internal::functions::tryBindSwResampleFunctionsFromLibrary(
          this->libSwresample, this->loggingFunction))
    this->swresample = functions.value();
  else
    return false;

  if (!checkLibraryVersion("swresample",
                           this->swresample.swresample_version(),
                           libraryVersions.swresample,
                           this->loggingFunction))
    return false;

  // AVCodec

  if (!this->tryLoadLibraryInPath(
          this->libAvcodec, absoluteDirectoryPath, "avcodec", libraryVersions.avcodec))
    return false;

  if (const auto functions = internal::functions::tryBindAVCodecFunctionsFromLibrary(
          this->libAvcodec, libraryVersions.avcodec, this->loggingFunction))
    this->avcodec = functions.value();
  else
    return false;

  if (!checkLibraryVersion("avcodec",
                           this->avcodec.avcodec_version(),
                           libraryVersions.avcodec,
                           this->loggingFunction))
    return false;

  // AVFormat

  if (!this->tryLoadLibraryInPath(
          this->libAvformat, absoluteDirectoryPath, "avformat", libraryVersions.avformat))
    return false;

  if (const auto functions = internal::functions::tryBindAVFormatFunctionsFromLibrary(
          this->libAvformat, this->loggingFunction))
    this->avformat = functions.value();
  else
    return false;

  if (!checkLibraryVersion("avformat",
                           this->avformat.avformat_version(),
                           libraryVersions.avformat,
                           this->loggingFunction))
    return false;

  // Success

  this->libraryVersions.avutil   = Version::fromFFmpegVersion(this->avutil.avutil_version());
  this->libraryVersions.avcodec  = Version::fromFFmpegVersion(this->avcodec.avcodec_version());
  this->libraryVersions.avformat = Version::fromFFmpegVersion(this->avformat.avformat_version());
  this->libraryVersions.swresample =
      Version::fromFFmpegVersion(this->swresample.swresample_version());

  this->avutil.av_log_set_callback(&FFmpegLibraries::avLogCallback);

  if (this->libraryVersions.avformat.major < 59)
    this->avformat.av_register_all();

  return true;
}

bool FFmpegLibraries::tryLoadLibraryInPath(SharedLibraryLoader &        lib,
                                           const std::filesystem::path &absoluteDirectoryPath,
                                           const std::string &          libName,
                                           const Version &              version)
{
  this->log(LogLevel::Info,
            "Trying to load library " + libName + " in path " + absoluteDirectoryPath.string());

  for (const auto &possibleLibName : getPossibleLibraryNames(libName, version.major))
  {
    if (!absoluteDirectoryPath.empty())
    {
      const auto absolutePathOrLibName = absoluteDirectoryPath / possibleLibName;
      const auto fileStatus            = std::filesystem::status(absolutePathOrLibName);

      if (fileStatus.type() == std::filesystem::file_type::not_found)
      {
        this->log(LogLevel::Info,
                  "Loading using lib name " + possibleLibName + " failed. Can not find file " +
                      absolutePathOrLibName.string());
        continue;
      }

      const auto success = lib.load(absolutePathOrLibName);
      this->log(LogLevel::Info,
                "Loading library " + absolutePathOrLibName.string() +
                    (success ? " succeded" : " failed"));
      if (success)
        return true;
    }
    else
    {
      const auto success = lib.load(possibleLibName);
      this->log(LogLevel::Info,
                "Loading library " + possibleLibName + (success ? " succeded" : " failed"));
      if (success)
        return true;
    }
  }
  return false;
};

void FFmpegLibraries::unloadAllLibraries()
{
  this->libAvutil.unload();
  this->libSwresample.unload();
  this->libAvcodec.unload();
  this->libAvformat.unload();
}

std::vector<LibraryInfo> FFmpegLibraries::getLibrariesInfo() const
{
  if (!this->libAvutil || !this->libSwresample || !this->libAvcodec || !this->libAvformat)
    return {};

  std::vector<LibraryInfo> infoPerLIbrary;

  auto addLibraryInfo = [&infoPerLIbrary](const char *                 name,
                                          const std::filesystem::path &path,
                                          const unsigned               ffmpegVersion) {
    const auto libraryVersion = Version::fromFFmpegVersion(ffmpegVersion);
    const auto version        = libraryVersion.toString();

    infoPerLIbrary.push_back(LibraryInfo({name, path, version}));
  };

  addLibraryInfo("AVFormat", this->libAvformat.getLibraryPath(), this->avformat.avformat_version());
  addLibraryInfo("AVCodec", this->libAvcodec.getLibraryPath(), this->avcodec.avcodec_version());
  addLibraryInfo("AVUtil", this->libAvutil.getLibraryPath(), this->avutil.avutil_version());
  addLibraryInfo(
      "SwResample", this->libSwresample.getLibraryPath(), this->swresample.swresample_version());

  return infoPerLIbrary;
}

void FFmpegLibraries::setLoggingFunction(const LoggingFunction loggingFunction)
{
  this->loggingFunction = loggingFunction;
}

void FFmpegLibraries::log(const LogLevel logLevel, const std::string &message) const
{
  if (this->loggingFunction)
    this->loggingFunction(logLevel, message);
}

std::string FFmpegLibraries::logListFFmpeg;

void FFmpegLibraries::avLogCallback(void *, int level, const char *fmt, va_list vargs)
{
  std::string message;
  va_list     vargs_copy;
  va_copy(vargs_copy, vargs);
  size_t len = vsnprintf(0, 0, fmt, vargs_copy);
  message.resize(len);
  vsnprintf(&message[0], len + 1, fmt, vargs);

  FFmpegLibraries::logListFFmpeg.append("Level " + std::to_string(level) + message);
}

} // namespace ffmpeg
