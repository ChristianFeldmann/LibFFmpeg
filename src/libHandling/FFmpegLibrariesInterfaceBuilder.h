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

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>

#include <memory>

namespace LibFFmpeg
{

struct LibrariesLoadingResult
{
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
  std::string                               errorMessage{};
  std::vector<std::string>                  loadingLog{};

  explicit operator bool() const { return this->librariesInterface.get() != nullptr; };
};

class FFmpegLibrariesInterfaceBuilder
{
public:
  FFmpegLibrariesInterfaceBuilder() = default;

  LibrariesLoadingResult tryLoadingOfLibraries();

  FFmpegLibrariesInterfaceBuilder &
  withAdditionalSearchPaths(const std::vector<std::filesystem::path> &additionalPath);
  FFmpegLibrariesInterfaceBuilder &withForcedReload();

private:
  std::vector<std::filesystem::path>      searchPaths{};
  bool                                    forceReload{false};
  std::weak_ptr<FFmpegLibrariesInterface> lastLoadedLibraries{};
};

} // namespace LibFFmpeg
