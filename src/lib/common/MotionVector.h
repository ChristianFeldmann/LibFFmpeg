/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "FFMpegLibrariesTypes.h"

namespace ffmpeg
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

} // namespace ffmpeg
