/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "FFmpegLibrariesBuilder.h"

#include "FFmpegLibraries.h"

namespace ffmpeg
{

LibrariesLoadingResult FFmpegLibrariesBuilder::tryLoadingOfLibraries()
{
  LibrariesLoadingResult result;

  if (this->forceReload)
    result.loadingLog.push_back("Library reload forced. Will skip cache check.");
  else
  {
    if (auto cachedFFmpegLibraries = this->lastLoadedLibraries.lock())
    {
      result.loadingLog.push_back("Cached libraries still loaded. Returning cached libraries.");
      result.ffmpegLibraries = cachedFFmpegLibraries;
      return result;
    }
  }

  if (this->searchPaths.empty())
  {
    result.loadingLog.push_back(
        "No search paths specified. We will only try the default empty path.");
    this->searchPaths.push_back("");
  }

  auto libraryInterface = std::make_shared<FFmpegLibraries>();

  for (const auto &path : this->searchPaths)
  {
    result.loadingLog.push_back("Trying to load the libraries in the path " + path.string());

    const auto [success, log] = libraryInterface->tryLoadFFmpegLibrariesInPath(path);
    result.loadingLog.insert(result.loadingLog.end(), log.begin(), log.end());

    if (success)
    {
      result.ffmpegLibraries = libraryInterface;
      return result;
    }
  }

  return result;
}

FFmpegLibrariesBuilder &FFmpegLibrariesBuilder::withAdditionalSearchPaths(
    const std::vector<std::filesystem::path> &additionalPath)
{
  this->searchPaths.insert(this->searchPaths.begin(), additionalPath.begin(), additionalPath.end());
  return *this;
}

FFmpegLibrariesBuilder &FFmpegLibrariesBuilder::withForcedReload()
{
  this->forceReload = true;
  return *this;
}

} // namespace ffmpeg
