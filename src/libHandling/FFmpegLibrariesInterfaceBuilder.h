/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
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
