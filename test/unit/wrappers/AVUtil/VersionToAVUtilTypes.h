/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <wrappers/RunTestForAllVersions.h>

#include <AVUtil/wrappers/AVFrameSideDataWrapperInternal.h>
#include <AVUtil/wrappers/AVMotionVectorConversionInternal.h>

namespace ffmpeg::avutil
{

using internal::avutil::AVFrameSideData_54;
using internal::avutil::AVFrameSideData_55;
using internal::avutil::AVFrameSideData_56;
using internal::avutil::AVFrameSideData_57;
using internal::avutil::AVFrameSideData_58;
using internal::avutil::AVMotionVector_54;
using internal::avutil::AVMotionVector_55_56_57_58;

template <FFmpegVersion V> constexpr auto avFrameSideDataTypeFromVersionFunc()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<AVFrameSideData_54>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<AVFrameSideData_55>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<AVFrameSideData_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<AVFrameSideData_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<AVFrameSideData_58>{};
}

template <FFmpegVersion V> constexpr auto avMotionVectorTypeFromVersionFunc()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<AVMotionVector_54>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x || V == FFmpegVersion::FFmpeg_4x ||
                V == FFmpegVersion::FFmpeg_5x || V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<AVMotionVector_55_56_57_58>{};
}

template <FFmpegVersion V>
using FrameSideDataType = typename decltype(avFrameSideDataTypeFromVersionFunc<V>())::type;

template <FFmpegVersion V>
using MotionVectorType = typename decltype(avMotionVectorTypeFromVersionFunc<V>())::type;

} // namespace ffmpeg::avutil
