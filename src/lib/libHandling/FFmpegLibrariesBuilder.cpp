/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "FFmpegLibrariesBuilder.h"

#include "FFmpegLibraries.h"

namespace libffmpeg
{

LibrariesLoadingResult FFmpegLibrariesBuilder::tryLoadingOfLibraries()
{
  LibrariesLoadingResult result;

  if (this->forceReload)
  {
    if (this->loggingFunction)
      this->loggingFunction(LogLevel::Info, "Library reload forced. Will skip cache check.");
  }
  else
  {
    if (auto cachedFFmpegLibraries = this->lastLoadedLibraries.lock())
    {
      if (this->loggingFunction)
        this->loggingFunction(LogLevel::Info,
                              "Cached libraries still loaded. Returning cached libraries.");
      result.ffmpegLibraries = cachedFFmpegLibraries;
      return result;
    }
  }

  auto lib = std::make_shared<FFmpegLibraries>();
  if (this->loggingFunction)
    lib->setLoggingFunction(this->loggingFunction, this->minimumLogLevel);

  auto paths = this->searchPaths;
  if (paths.empty())
  {
    lib->log(LogLevel::Info, "No search paths specified. We will only try the default empty path.");
    paths.push_back("");
  }

#if (defined(__APPLE__) && defined(__arm64__))
  // On apple silicone, the libraries that were installed through homebrew are not automatically
  // in the library search path. We will search the homebrew library path manually.
  // Issue: https://github.com/Homebrew/brew/issues/13481
  paths.push_back("/opt/homebrew/lib");
#elif (defined(__APPLE__) && !defined(__arm64__))
  // On macos on Intel, it can also happen that the default directory that homebrew libraries
  // are installed in is not in the search path.
  // https://apple.stackexchange.com/questions/40704/homebrew-installed-libraries-how-do-i-use-them
  paths.push_back("/usr/local/lib");
#endif

  for (const auto &path : paths)
  {
    lib->log(LogLevel::Info, "Trying to load the libraries in the path " + path.string());

    if (lib->tryLoadFFmpegLibrariesInPath(path))
    {
      result.ffmpegLibraries = lib;
      return result;
    }
  }

  return result;
}

FFmpegLibrariesBuilder &
FFmpegLibrariesBuilder::withAdditionalSearchPaths(const std::vector<Path> &additionalPath)
{
  this->searchPaths.insert(this->searchPaths.begin(), additionalPath.begin(), additionalPath.end());
  return *this;
}

FFmpegLibrariesBuilder &FFmpegLibrariesBuilder::withForcedReload()
{
  this->forceReload = true;
  return *this;
}

FFmpegLibrariesBuilder &
FFmpegLibrariesBuilder::withLoggingFunction(const LoggingFunction loggingCallback,
                                            const LogLevel        minimumLogLevel)
{
  this->loggingFunction = loggingCallback;
  this->minimumLogLevel = minimumLogLevel;
  return *this;
}

} // namespace libffmpeg
