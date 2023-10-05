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

#include "FFMpegLibrariesTypes.h"

namespace LibFFmpeg
{

struct MotionVector
{
  int32_t  source{};
  uint8_t  w{};
  uint8_t  h{};
  int16_t  src_x{};
  int16_t  src_y{};
  int16_t  dst_x{};
  int16_t  dst_y{};
  uint64_t flags{};
  // The following may be invalid (-1) in older ffmpeg versions)
  int32_t  motion_x{};
  int32_t  motion_y{};
  uint16_t motion_scale{};
};

std::vector<MotionVector>
parseMotionData(const LibraryVersions &libraryVersions, const uint8_t *data, const size_t dataSize);

} // namespace LibFFmpeg
