/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVFormat/wrappers/AVCodecParametersWrapperInternal.h>
#include <AVFormat/wrappers/AVInputFormatWrapperInternal.h>
#include <AVFormat/wrappers/AVStreamWrapperInternal.h>
#include <wrappers/RunTestForAllVersions.h>

namespace ffmpeg::avformat
{

template <FFmpegVersion V> constexpr auto avCodecParametersTypeFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avformat::AVCodecParameters_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avformat::AVCodecParameters_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avformat::AVCodecParameters_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avformat::AVCodecParameters_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avformat::AVCodecParameters_60>{};
}

template <FFmpegVersion V>
using AVCodecParametersType = typename decltype(avCodecParametersTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avStreamTypeFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avformat::AVStream_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avformat::AVStream_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avformat::AVStream_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avformat::AVStream_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avformat::AVStream_60>{};
}

template <FFmpegVersion V> using AVStreamType = typename decltype(avStreamTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avInputFormatTypeFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<internal::avformat::AVInputFormat_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<internal::avformat::AVInputFormat_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<internal::avformat::AVInputFormat_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<internal::avformat::AVInputFormat_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<internal::avformat::AVInputFormat_60>{};
}

template <FFmpegVersion V>
using AVInputFormatType = typename decltype(avInputFormatTypeFunction<V>())::type;

} // namespace ffmpeg::avformat
