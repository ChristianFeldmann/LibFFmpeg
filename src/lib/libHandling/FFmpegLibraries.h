/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "IFFmpegLibraries.h"

#include <common/Logging.h>
#include <libHandling/SharedLibraryLoader.h>

#include <mutex>

namespace ffmpeg
{

class FFmpegLibraries : public IFFmpegLibraries
{
public:
  FFmpegLibraries();
  ~FFmpegLibraries();

  std::vector<LibraryInfo> getLibrariesInfo() const override;
  LibraryVersions          getLibrariesVersion() const override { return this->libraryVersions; }

  void setLoggingFunction(const LoggingFunction loggingFunction, const LogLevel minimumLogLevel);

  void log(const LogLevel logLevel, const std::string &message) const override;

private:
  [[nodiscard]] bool tryLoadFFmpegLibrariesInPath(const Path &path);

  [[nodiscard]] bool
                     tryLoadLibrariesBindFunctionsAndCheckVersions(const Path &           directory,
                                                                   const LibraryVersions &libraryVersions);
  [[nodiscard]] bool tryLoadBindAndCheckAVUtil(const Path &directory, const Version version);
  [[nodiscard]] bool tryLoadBindAndCheckSWResample(const Path &directory, const Version version);
  [[nodiscard]] bool tryLoadBindAndCheckAVCodec(const Path &directory, const Version version);
  [[nodiscard]] bool tryLoadBindAndCheckAVFormat(const Path &directory, const Version version);

  [[nodiscard]] bool tryLoadLibraryInPath(SharedLibraryLoader &lib,
                                          const Path &         directory,
                                          const std::string &  libName,
                                          const Version &      version);

  void unloadAllLibraries();

  SharedLibraryLoader libAvutil;
  SharedLibraryLoader libSwresample;
  SharedLibraryLoader libAvcodec;
  SharedLibraryLoader libAvformat;

  LibraryVersions libraryVersions{};
  void            getLibraryVersionsFromLoadedLibraries();

  void               connectAVLoggingCallback();
  void               disconnectAVLoggingCallback();
  mutable std::mutex loggingMutex;

  LoggingFunction    loggingFunction;
  std::optional<int> loggingFunctionIndex;

  friend class FFmpegLibrariesBuilder;
};

} // namespace ffmpeg
