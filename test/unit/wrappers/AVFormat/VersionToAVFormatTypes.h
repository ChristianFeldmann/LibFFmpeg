/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVFormat/wrappers/AVCodecParametersWrapperInternal.h>
#include <wrappers/RunTestForAllVersions.h>

namespace ffmpeg::avformat
{

using internal::avformat::AVCodecParameters_56;
using internal::avformat::AVCodecParameters_57;
using internal::avformat::AVCodecParameters_58;
using internal::avformat::AVCodecParameters_59;
using internal::avformat::AVCodecParameters_60;

template <FFmpegVersion V> constexpr auto avCodecParametersTypeFunction()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return TypeWrapper<AVCodecParameters_56>{};
  if constexpr (V == FFmpegVersion::FFmpeg_3x)
    return TypeWrapper<AVCodecParameters_57>{};
  if constexpr (V == FFmpegVersion::FFmpeg_4x)
    return TypeWrapper<AVCodecParameters_58>{};
  if constexpr (V == FFmpegVersion::FFmpeg_5x)
    return TypeWrapper<AVCodecParameters_59>{};
  if constexpr (V == FFmpegVersion::FFmpeg_6x)
    return TypeWrapper<AVCodecParameters_60>{};
}

template <FFmpegVersion V>
using AVCodecParametersType = typename decltype(avCodecParametersTypeFunction<V>())::type;

} // namespace ffmpeg::avformat
