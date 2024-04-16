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

bool checkLibraryVersion(const std::string     &libName,
                         unsigned               ffmpegVersionOfLoadedLibrary,
                         const Version         &expectedVersion,
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

Path getAbsolutePath(const Path &path)
{
  if (path == "." || path == "./")
    return std::filesystem::current_path();

  return std::filesystem::absolute(path);
}

constexpr auto                                           MAX_NR_REGISTERED_CALLBACKS = 3;
std::array<LoggingFunction, MAX_NR_REGISTERED_CALLBACKS> staticCallbackVector;

void logToLoggingFunction(
    LoggingFunction &loggingFunction, void *ptr, int level, const char *fmt, va_list vargs)
{
  if (!loggingFunction)
    return;

  std::string message;
  va_list     vargs_copy;
  va_copy(vargs_copy, vargs);
  size_t len = vsnprintf(0, 0, fmt, vargs_copy);
  message.resize(len);
  vsnprintf(&message[0], len + 1, fmt, vargs);

  auto logLevel = LogLevel::Debug;
  if (level <= 16)
    logLevel = LogLevel::Error;
  if (level <= 32)
    logLevel = LogLevel::Info;

  loggingFunction(logLevel, message);
}

void staticLogFunction0(void *ptr, int level, const char *fmt, va_list vargs)
{
  logToLoggingFunction(staticCallbackVector[0], ptr, level, fmt, vargs);
}

void staticLogFunction1(void *ptr, int level, const char *fmt, va_list vargs)
{
  logToLoggingFunction(staticCallbackVector[1], ptr, level, fmt, vargs);
}

void staticLogFunction2(void *ptr, int level, const char *fmt, va_list vargs)
{
  logToLoggingFunction(staticCallbackVector[2], ptr, level, fmt, vargs);
}

auto setStaticLoggingCallback(ffmpeg::internal::functions::AvUtilFunctions &avutilFunctions,
                              LoggingFunction                               loggingFunction)
{
  int callbackCounter = 0;
  while (callbackCounter < MAX_NR_REGISTERED_CALLBACKS)
  {
    if (!staticCallbackVector.at(callbackCounter))
    {
      staticCallbackVector[callbackCounter] = loggingFunction;
      if (callbackCounter == 0)
        avutilFunctions.av_log_set_callback(staticLogFunction0);
      else if (callbackCounter == 1)
        avutilFunctions.av_log_set_callback(staticLogFunction1);
      else if (callbackCounter == 2)
        avutilFunctions.av_log_set_callback(staticLogFunction2);
      return;
    }
    ++callbackCounter;
  }

  loggingFunction(LogLevel::Error,
                  "Error setting av logging callback. The limit of static callbacks was reached.");
}

} // namespace

FFmpegLibraries::FFmpegLibraries()
{
  std::lock_guard<std::mutex> lock(this->loggingMutex);
  this->loggingFunction = [](const LogLevel, const std::string &) {};
}

FFmpegLibraries::~FFmpegLibraries()
{
  this->disconnectAVLoggingCallback();
}

bool FFmpegLibraries::tryLoadFFmpegLibrariesInPath(const Path &path)
{
  Path directory;
  if (!path.empty())
  {
    if (!std::filesystem::exists(path))
    {
      this->log(LogLevel::Error, "The given path (" + path.string() + ") could not be found");
      return false;
    }

    directory = getAbsolutePath(path);
    this->log(LogLevel::Info, "Using absolute path " + directory.string());
  }

  for (const auto &version : SupportedFFmpegVersions)
  {
    this->unloadAllLibraries();
    this->log(LogLevel::Debug, "Unload libraries");

    if (this->tryLoadLibrariesBindFunctionsAndCheckVersions(directory, version))
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

bool FFmpegLibraries::tryLoadLibrariesBindFunctionsAndCheckVersions(const Path &directory,
                                                                    const LibraryVersions &versions)
{
  if (!this->tryLoadBindAndCheckAVUtil(directory, versions.avutil) ||
      !this->tryLoadBindAndCheckSWResample(directory, versions.swresample) ||
      !this->tryLoadBindAndCheckAVCodec(directory, versions.avcodec) ||
      !this->tryLoadBindAndCheckAVFormat(directory, versions.avformat))
    return false;

  this->getLibraryVersionsFromLoadedLibraries();
  this->connectAVLoggingCallback();

  if (this->libraryVersions.avformat.major < 59)
    this->avformat.av_register_all();

  return true;
}

bool FFmpegLibraries::tryLoadBindAndCheckAVUtil(const Path &directory, const Version version)
{
  if (!this->tryLoadLibraryInPath(this->libAvutil, directory, "avutil", version))
    return false;

  if (const auto functions = internal::functions::tryBindAVUtilFunctionsFromLibrary(
          this->libAvutil, this->loggingFunction))
    this->avutil = functions.value();
  else
    return false;

  return checkLibraryVersion(
      "avUtil", this->avutil.avutil_version(), version, this->loggingFunction);
}

bool FFmpegLibraries::tryLoadBindAndCheckSWResample(const Path &directory, const Version version)
{
  if (!this->tryLoadLibraryInPath(this->libSwresample, directory, "swresample", version))
    return false;

  if (const auto functions = internal::functions::tryBindSwResampleFunctionsFromLibrary(
          this->libSwresample, this->loggingFunction))
    this->swresample = functions.value();
  else
    return false;

  return checkLibraryVersion(
      "swresample", this->swresample.swresample_version(), version, this->loggingFunction);
}

bool FFmpegLibraries::tryLoadBindAndCheckAVCodec(const Path &directory, const Version version)
{
  if (!this->tryLoadLibraryInPath(this->libAvcodec, directory, "avcodec", version))
    return false;

  if (const auto functions = internal::functions::tryBindAVCodecFunctionsFromLibrary(
          this->libAvcodec, version, this->loggingFunction))
    this->avcodec = functions.value();
  else
    return false;

  return checkLibraryVersion(
      "avcodec", this->avcodec.avcodec_version(), version, this->loggingFunction);
}

bool FFmpegLibraries::tryLoadBindAndCheckAVFormat(const Path &directory, const Version version)
{
  if (!this->tryLoadLibraryInPath(this->libAvformat, directory, "avformat", version))
    return false;

  if (const auto functions = internal::functions::tryBindAVFormatFunctionsFromLibrary(
          this->libAvformat, this->loggingFunction))
    this->avformat = functions.value();
  else
    return false;

  return checkLibraryVersion(
      "avformat", this->avformat.avformat_version(), version, this->loggingFunction);
}

bool FFmpegLibraries::tryLoadLibraryInPath(SharedLibraryLoader &lib,
                                           const Path          &directory,
                                           const std::string   &libName,
                                           const Version       &version)
{
  this->log(LogLevel::Info, "Trying to load library " + libName + " in path " + directory.string());

  for (const auto &possibleLibName : getPossibleLibraryNames(libName, version.major))
  {
    if (!directory.empty())
    {
      const auto absolutePathOrLibName = directory / possibleLibName;
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

  auto addLibraryInfo =
      [&infoPerLIbrary](const char *name, const Path &path, const unsigned ffmpegVersion)
  {
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

void FFmpegLibraries::setLoggingFunction(const LoggingFunction function,
                                         const LogLevel        minimumLogLevel)
{
  {
    std::lock_guard<std::mutex> lock(this->loggingMutex);
    this->loggingFunction =
        [this, function, minimumLogLevel](const LogLevel logLevel, const std::string &message)
    {
      if (function && logLevel >= minimumLogLevel)
        function(logLevel, message);
    };
  }

  this->log(LogLevel::Info, "Logging function set successfully");
}

void FFmpegLibraries::log(const LogLevel logLevel, const std::string &message) const
{
  std::lock_guard<std::mutex> lock(this->loggingMutex);
  this->loggingFunction(logLevel, message);
}

void FFmpegLibraries::getLibraryVersionsFromLoadedLibraries()
{
  this->libraryVersions.avutil   = Version::fromFFmpegVersion(this->avutil.avutil_version());
  this->libraryVersions.avcodec  = Version::fromFFmpegVersion(this->avcodec.avcodec_version());
  this->libraryVersions.avformat = Version::fromFFmpegVersion(this->avformat.avformat_version());
  this->libraryVersions.swresample =
      Version::fromFFmpegVersion(this->swresample.swresample_version());
}

void FFmpegLibraries::connectAVLoggingCallback()
{
  this->log(LogLevel::Info, "Setting up av logging callback");
  std::lock_guard<std::mutex> lock(this->loggingMutex);

  setStaticLoggingCallback(this->avutil, this->loggingFunction);
}

void FFmpegLibraries::disconnectAVLoggingCallback()
{
  this->log(LogLevel::Info, "Disconnectiong av logging callback");
  std::lock_guard<std::mutex> lock(this->loggingMutex);

  this->avutil.av_log_set_callback(
      this->avutil.av_log_default_callback.target<void(void *, int, const char *, va_list)>());
}

} // namespace ffmpeg
