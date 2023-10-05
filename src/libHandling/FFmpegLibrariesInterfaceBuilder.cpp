/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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