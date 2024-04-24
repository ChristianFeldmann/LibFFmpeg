/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVFormat/wrappers/AVFormatContextWrapperInternal.h>
#include <AVFormat/wrappers/AVInputFormatWrapperInternal.h>
#include <AVFormat/wrappers/AVStreamWrapperInternal.h>
#include <wrappers/RunTestForAllVersions.h>

namespace libffmpeg::avformat
{

namespace
{

#define RETURN_AVFORMAT_TYPE_WRAPPER_FOR_VERSION_V(wrapperNamePrefix)                              \
  {                                                                                                \
    if constexpr (V == FFmpegVersion::FFmpeg_2x)                                                   \
      return TypeWrapper<internal::avformat::wrapperNamePrefix##_56>{};                            \
    if constexpr (V == FFmpegVersion::FFmpeg_3x)                                                   \
      return TypeWrapper<internal::avformat::wrapperNamePrefix##_57>{};                            \
    if constexpr (V == FFmpegVersion::FFmpeg_4x)                                                   \
      return TypeWrapper<internal::avformat::wrapperNamePrefix##_58>{};                            \
    if constexpr (V == FFmpegVersion::FFmpeg_5x)                                                   \
      return TypeWrapper<internal::avformat::wrapperNamePrefix##_59>{};                            \
    if constexpr (V == FFmpegVersion::FFmpeg_6x)                                                   \
      return TypeWrapper<internal::avformat::wrapperNamePrefix##_60>{};                            \
  }

} // namespace

template <FFmpegVersion V> constexpr auto avStreamTypeFunction()
{
  RETURN_AVFORMAT_TYPE_WRAPPER_FOR_VERSION_V(AVStream);
}

template <FFmpegVersion V> using AVStreamType = typename decltype(avStreamTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avInputFormatTypeFunction()
{
  RETURN_AVFORMAT_TYPE_WRAPPER_FOR_VERSION_V(AVInputFormat);
}

template <FFmpegVersion V>
using AVInputFormatType = typename decltype(avInputFormatTypeFunction<V>())::type;

template <FFmpegVersion V> constexpr auto avFormatContextTypeFunction()
{
  RETURN_AVFORMAT_TYPE_WRAPPER_FOR_VERSION_V(AVFormatContext);
}

template <FFmpegVersion V>
using AVFormatContextType = typename decltype(avFormatContextTypeFunction<V>())::type;

} // namespace libffmpeg::avformat
