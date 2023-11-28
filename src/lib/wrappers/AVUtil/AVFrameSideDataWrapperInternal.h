/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace ffmpeg::internal::avutil
{

struct AVFrameSideData_54
{
  AVFrameSideDataType type;
  uint8_t            *data;
  int                 size;
  AVDictionary       *metadata;
  AVBufferRef        *buf;
};

typedef AVFrameSideData_54 AVFrameSideData_55;
typedef AVFrameSideData_54 AVFrameSideData_56;

struct AVFrameSideData_57
{
  AVFrameSideDataType type;
  uint8_t            *data;
  size_t              size;
  AVDictionary       *metadata;
  AVBufferRef        *buf;
};

typedef AVFrameSideData_57 AVFrameSideData_58;

} // namespace ffmpeg::internal::avutil
