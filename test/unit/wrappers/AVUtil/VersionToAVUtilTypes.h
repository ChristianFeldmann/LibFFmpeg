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

namespace
{

#define RETURN_AVUTIL_TYPE_WRAPPER_FOR_VERSION_V(wrapperNamePrefix)                                \
  {                                                                                                \
    if constexpr (V == FFmpegVersion::FFmpeg_2x)                                                   \
      return TypeWrapper<internal::avutil::wrapperNamePrefix##_54>{};                              \
    if constexpr (V == FFmpegVersion::FFmpeg_3x)                                                   \
      return TypeWrapper<internal::avutil::wrapperNamePrefix##_55>{};                              \
    if constexpr (V == FFmpegVersion::FFmpeg_4x)                                                   \
      return TypeWrapper<internal::avutil::wrapperNamePrefix##_56>{};                              \
    if constexpr (V == FFmpegVersion::FFmpeg_5x)                                                   \
      return TypeWrapper<internal::avutil::wrapperNamePrefix##_57>{};                              \
    if constexpr (V == FFmpegVersion::FFmpeg_6x)                                                   \
      return TypeWrapper<internal::avutil::wrapperNamePrefix##_58>{};                              \
  }

} // namespace

template <FFmpegVersion V> constexpr auto avFrameSideDataTypeFromVersionFunc()
{
  RETURN_AVUTIL_TYPE_WRAPPER_FOR_VERSION_V(AVFrameSideData);
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
  RETURN_AVUTIL_TYPE_WRAPPER_FOR_VERSION_V(AVPixFmtDescriptor);
}

template <FFmpegVersion V>
using AVPixFmtDescriptorType = typename decltype(avPixFmtDescriptorFromVersionFunc<V>())::type;

template <FFmpegVersion V> constexpr auto avFrameFromVersionFunc()
{
  RETURN_AVUTIL_TYPE_WRAPPER_FOR_VERSION_V(AVFrame);
}

template <FFmpegVersion V> using AVFrameType = typename decltype(avFrameFromVersionFunc<V>())::type;

} // namespace ffmpeg::avutil
