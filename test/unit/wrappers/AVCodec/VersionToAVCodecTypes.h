/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVCodecContextWrapperInternal.h>
#include <AVCodec/wrappers/AVCodecDescriptorConversionInternal.h>
#include <AVCodec/wrappers/AVCodecWrapperInternal.h>
#include <AVCodec/wrappers/AVPacketWrapperInternal.h>
#include <wrappers/RunTestForAllVersions.h>

namespace ffmpeg::avcodec
{

template <FFmpegVersion V> constexpr auto avCodecContextFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avcodec::AVCodecContext_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avcodec::AVCodecContext_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avcodec::AVCodecContext_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avcodec::AVCodecContext_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avcodec::AVCodecContext_60>{};
}

template <FFmpegVersion V>
using AVCodecContextType = typename decltype(avCodecContextFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avPacketTypeFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avcodec::AVPacket_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avcodec::AVPacket_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avcodec::AVPacket_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avcodec::AVPacket_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avcodec::AVPacket_60>{};
}

template <FFmpegVersion V> using AVPacketType = typename decltype(avPacketTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avCodecTypeFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avcodec::AVCodec_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avcodec::AVCodec_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avcodec::AVCodec_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avcodec::AVCodec_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avcodec::AVCodec_60>{};
}

template <FFmpegVersion V> using AVCodecType = typename decltype(avCodecTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avCodecDescriptorTypeFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avcodec::AVCodecDescriptor_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avcodec::AVCodecDescriptor_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avcodec::AVCodecDescriptor_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avcodec::AVCodecDescriptor_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avcodec::AVCodecDescriptor_60>{};
}

template <FFmpegVersion V>
using AVCodecDescriptorType = typename decltype(avCodecDescriptorTypeFunction<V>())::type;

} // namespace ffmpeg::avcodec
