/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "FFmpegLibrariesInterfaceBuilder.h"

namespace LibFFmpeg
{

LibrariesLoadingResult FFmpegLibrariesInterfaceBuilder::tryLoadingOfLibraries()
{
  LibrariesLoadingResult result;

  if (this->forceReload)
    result.loadingLog.push_back("Library reload forced. Will skip cache check.");
  else
  {
    if (auto cachedLibrariesInterface = this->lastLoadedLibraries.lock())
    {
      result.loadingLog.push_back("Cached libraries still loaded. Returning cached libraries.");
      result.librariesInterface = cachedLibrariesInterface;
      return result;
    }
  }

  if (this->searchPaths.empty())
  {
    result.loadingLog.push_back(
        "No search paths specified. We will only try the default empty path.");
    this->searchPaths.push_back("");
  }

  auto libraryInterface = std::make_shared<FFmpegLibrariesInterface>();

  for (const auto &path : this->searchPaths)
  {
    result.loadingLog.push_back("Trying to load the libraries in the path " + path.string());

    const auto [success, log] = libraryInterface->tryLoadFFmpegLibrariesInPath(path);
    result.loadingLog.insert(result.loadingLog.begin(), log.begin(), log.end());

    if (success)
    {
      result.librariesInterface = libraryInterface;
      return result;
    }
  }

  return result;
}

FFmpegLibrariesInterfaceBuilder &FFmpegLibrariesInterfaceBuilder::withAdditionalSearchPaths(
    const std::vector<std::filesystem::path> &additionalPath)
{
  this->searchPaths.insert(this->searchPaths.begin(), additionalPath.begin(), additionalPath.end());
  return *this;
}

FFmpegLibrariesInterfaceBuilder &FFmpegLibrariesInterfaceBuilder::withForcedReload()
{
  this->forceReload = true;
  return *this;
}

} // namespace LibFFmpeg