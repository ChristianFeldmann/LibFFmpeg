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

#include <common/FFMpegLibrariesTypes.h>

namespace LibFFmpeg
{

class AVInputFormatWrapper
{
public:
  AVInputFormatWrapper() = default;
  AVInputFormatWrapper(AVInputFormat *avInputFormat, const LibraryVersions &libraryVersions);

  explicit operator bool() const { return this->avInputFormat != nullptr; };

private:
  // Update all private values from the AVCodecContext
  void update();

  // These are here for debugging purposes.
  std::string name{};
  std::string long_name{};
  int         flags{};
  std::string extensions{};
  std::string mime_type{};

  AVInputFormat * avInputFormat{};
  LibraryVersions libraryVersions{};
};

} // namespace LibFFmpeg
