/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <cstdint>

namespace ffmpeg::internal::avutil
{

// This is important so that the compiler does not add padding if a struct is allocated.
// In the tests, we create some of these. If this is not done, sizeof() will return unexpected
// values.
#pragma pack(push, 1)

struct AVMotionVector_54
{
  int32_t  source;
  uint8_t  w, h;
  int16_t  src_x, src_y;
  int16_t  dst_x, dst_y;
  uint64_t flags;
};

struct AVMotionVector_55_56_57_58
{
  int32_t  source;
  uint8_t  w, h;
  int16_t  src_x, src_y;
  int16_t  dst_x, dst_y;
  uint64_t flags;
  int32_t  motion_x, motion_y;
  uint16_t motion_scale;
};

#pragma pack(pop)

} // namespace ffmpeg::internal::avutil
