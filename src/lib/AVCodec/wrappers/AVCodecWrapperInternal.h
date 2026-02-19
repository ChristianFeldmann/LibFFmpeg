/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

#include "AVChannelInternal.h"

namespace libffmpeg::internal::avcodec
{

struct AVCodec_56
{
  const char           *name{};
  const char           *long_name{};
  AVMediaType           type{};
  AVCodecID             id{};
  int                   capabilities{};
  const AVRational     *supported_framerates{};
  const AVPixelFormat  *pix_fmts{};
  const int            *supported_samplerates{};
  const AVSampleFormat *sample_fmts{};
  const uint64_t       *channel_layouts{};
  uint8_t               max_lowres{};
  const AVClass        *priv_class{};

  // Actually, there is more here, but nothing more of the public API
};

using AVCodec_57 = AVCodec_56;
using AVCodec_58 = AVCodec_56;

struct AVCodec_59
{
  const char           *name{};
  const char           *long_name{};
  AVMediaType           type{};
  AVCodecID             id{};
  int                   capabilities{};
  uint8_t               max_lowres{};
  const AVRational     *supported_framerates{};
  const AVPixelFormat  *pix_fmts{};
  const int            *supported_samplerates{};
  const AVSampleFormat *sample_fmts{};
  const uint64_t       *channel_layouts{};
  const AVClass        *priv_class{};

  // Actually, there is more here, but nothing more of the public API
};

struct AVCodec_60
{
  const char            *name{};
  const char            *long_name{};
  AVMediaType            type{};
  AVCodecID              id{};
  int                    capabilities{};
  uint8_t                max_lowres{};
  const AVRational      *supported_framerates{};
  const AVPixelFormat   *pix_fmts{};
  const int             *supported_samplerates{};
  const AVSampleFormat  *sample_fmts{};
  const uint64_t        *deprecated_channel_layouts{}; // Actually named channel_layouts
  const AVClass         *priv_class{};
  const AVProfile       *profiles{};
  const char            *wrapper_name{};
  const AVChannelLayout *channel_layouts{}; // Actual name ch_layouts
};

struct AVCodec_61
{
  const char            *name{};
  const char            *long_name{};
  AVMediaType            type{};
  AVCodecID              id{};
  int                    capabilities{};
  uint8_t                max_lowres{};
  const AVRational      *supported_framerates{};
  const AVPixelFormat   *pix_fmts{};
  const int             *supported_samplerates{};
  const AVSampleFormat  *sample_fmts{};
  const AVClass         *priv_class{};
  const AVProfile       *profiles{};
  const char            *wrapper_name{};
  const AVChannelLayout *channel_layouts{};
};

using AVCodec_62 = AVCodec_61;

} // namespace libffmpeg::internal::avcodec
