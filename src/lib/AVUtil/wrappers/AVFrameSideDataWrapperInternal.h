/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace libffmpeg::internal::avutil
{

struct AVFrameSideData_54
{
  AVFrameSideDataType type;
  uint8_t            *data;
  int                 size;
  AVDictionary       *metadata;
  AVBufferRef        *buf;
};

using AVFrameSideData_55 = AVFrameSideData_54;
using AVFrameSideData_56 = AVFrameSideData_54;

struct AVFrameSideData_57
{
  AVFrameSideDataType type;
  uint8_t            *data;
  size_t              size;
  AVDictionary       *metadata;
  AVBufferRef        *buf;
};

using AVFrameSideData_58 = AVFrameSideData_57;
using AVFrameSideData_59 = AVFrameSideData_57;
using AVFrameSideData_60 = AVFrameSideData_57;

} // namespace libffmpeg::internal::avutil
