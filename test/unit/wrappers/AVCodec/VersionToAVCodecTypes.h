/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVCodecContextWrapperInternal.h>
#include <AVCodec/wrappers/AVCodecDescriptorConversionInternal.h>
#include <AVCodec/wrappers/AVCodecParametersWrapperInternal.h>
#include <AVCodec/wrappers/AVCodecWrapperInternal.h>
#include <AVCodec/wrappers/AVPacketWrapperInternal.h>
#include <wrappers/RunTestForAllVersions.h>

namespace libffmpeg::avcodec
{

namespace
{

#define RETURN_AVCODEC_TYPE_WRAPPER_FOR_VERSION_V(wrapperNamePrefix)                               \
  {                                                                                                \
    if constexpr (V == FFmpegVersion::FFmpeg_2x)                                                   \
      return TypeWrapper<internal::avcodec::wrapperNamePrefix##_56>{};                             \
    if constexpr (V == FFmpegVersion::FFmpeg_3x)                                                   \
      return TypeWrapper<internal::avcodec::wrapperNamePrefix##_57>{};                             \
    if constexpr (V == FFmpegVersion::FFmpeg_4x)                                                   \
      return TypeWrapper<internal::avcodec::wrapperNamePrefix##_58>{};                             \
    if constexpr (V == FFmpegVersion::FFmpeg_5x)                                                   \
      return TypeWrapper<internal::avcodec::wrapperNamePrefix##_59>{};                             \
    if constexpr (V == FFmpegVersion::FFmpeg_6x)                                                   \
      return TypeWrapper<internal::avcodec::wrapperNamePrefix##_60>{};                             \
    if constexpr (V == FFmpegVersion::FFmpeg_7x)                                                   \
      return TypeWrapper<internal::avcodec::wrapperNamePrefix##_61>{};                             \
    if constexpr (V == FFmpegVersion::FFmpeg_8x)                                                   \
      return TypeWrapper<internal::avcodec::wrapperNamePrefix##_62>{};                             \
    else                                                                                           \
      throw std::runtime_error("Invalid FFmpeg version");                                          \
  }

} // namespace

template <FFmpegVersion V> constexpr auto avCodecParametersTypeFunction()
{
  RETURN_AVCODEC_TYPE_WRAPPER_FOR_VERSION_V(AVCodecParameters);
}

template <FFmpegVersion V>
using AVCodecParametersType = typename decltype(avCodecParametersTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avCodecContextFunction()
{
  RETURN_AVCODEC_TYPE_WRAPPER_FOR_VERSION_V(AVCodecContext);
}

template <FFmpegVersion V>
using AVCodecContextType = typename decltype(avCodecContextFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avPacketTypeFunction()
{
  RETURN_AVCODEC_TYPE_WRAPPER_FOR_VERSION_V(AVPacket);
}

template <FFmpegVersion V> using AVPacketType = typename decltype(avPacketTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avCodecTypeFunction()
{
  RETURN_AVCODEC_TYPE_WRAPPER_FOR_VERSION_V(AVCodec);
}

template <FFmpegVersion V> using AVCodecType = typename decltype(avCodecTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avCodecDescriptorTypeFunction()
{
  RETURN_AVCODEC_TYPE_WRAPPER_FOR_VERSION_V(AVCodecDescriptor);
}

template <FFmpegVersion V>
using AVCodecDescriptorType = typename decltype(avCodecDescriptorTypeFunction<V>())::type;

} // namespace libffmpeg::avcodec
