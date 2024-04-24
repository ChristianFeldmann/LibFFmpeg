/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Logging.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace libffmpeg
{

struct LibrariesLoadingResult
{
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};

  explicit operator bool() const { return this->ffmpegLibraries.get() != nullptr; };
};

class FFmpegLibrariesBuilder
{
public:
  FFmpegLibrariesBuilder() = default;

  LibrariesLoadingResult tryLoadingOfLibraries();

  FFmpegLibrariesBuilder &withAdditionalSearchPaths(const std::vector<Path> &additionalPath);
  FFmpegLibrariesBuilder &withForcedReload();
  FFmpegLibrariesBuilder &withLoggingFunction(const LoggingFunction loggingFunction,
                                              const LogLevel        minimumLogLevel);

private:
  std::vector<Path>               searchPaths{};
  bool                            forceReload{false};
  std::weak_ptr<IFFmpegLibraries> lastLoadedLibraries{};
  LoggingFunction                 loggingFunction{};
  LogLevel                        minimumLogLevel{LogLevel::Error};
};

} // namespace libffmpeg
