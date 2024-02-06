/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <wrappers/RunTestForAllVersions.h>

#include <AVUtil/wrappers/AVFrameSideDataWrapperInternal.h>
#include <AVUtil/wrappers/AVFrameWrapperInternal.h>
#include <AVUtil/wrappers/AVMotionVectorConversionInternal.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversionInternal.h>

namespace ffmpeg::avutil
{

template <FFmpegVersion V> constexpr auto avFrameSideDataTypeFromVersionFunc()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avutil::AVFrameSideData_54>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avutil::AVFrameSideData_55>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avutil::AVFrameSideData_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avutil::AVFrameSideData_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avutil::AVFrameSideData_58>{};
}

template <FFmpegVersion V>
using FrameSideDataType = typename decltype(avFrameSideDataTypeFromVersionFunc<V>())::type;

template <FFmpegVersion V> constexpr auto avMotionVectorTypeFromVersionFunc()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avutil::AVMotionVector_54>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x || V == FFmpegVersion::FFmpeg_4x ||
                V == FFmpegVersion::FFmpeg_5x || V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avutil::AVMotionVector_55_56_57_58>{};
}

template <FFmpegVersion V>
using MotionVectorType = typename decltype(avMotionVectorTypeFromVersionFunc<V>())::type;

template <FFmpegVersion V> constexpr auto avPixFmtDescriptorFromVersionFunc()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avutil::AVPixFmtDescriptor_54>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avutil::AVPixFmtDescriptor_55>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avutil::AVPixFmtDescriptor_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avutil::AVPixFmtDescriptor_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avutil::AVPixFmtDescriptor_58>{};
}

template <FFmpegVersion V>
using AVPixFmtDescriptorType = typename decltype(avPixFmtDescriptorFromVersionFunc<V>())::type;

template <FFmpegVersion V> constexpr auto avFrameFromVersionFunc()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avutil::AVFrame_54>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avutil::AVFrame_55>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avutil::AVFrame_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avutil::AVFrame_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avutil::AVFrame_58>{};
}

template <FFmpegVersion V> using AVFrameType = typename decltype(avFrameFromVersionFunc<V>())::type;

} // namespace ffmpeg::avutil
