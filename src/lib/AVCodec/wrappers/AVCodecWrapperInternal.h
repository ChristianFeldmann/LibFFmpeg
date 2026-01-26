/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace libffmpeg::internal::avcodec
{

struct AVCodec_56
{
  const char           *name;
  const char           *long_name;
  AVMediaType           type;
  AVCodecID             id;
  int                   capabilities;
  const AVRational     *supported_framerates;
  const AVPixelFormat  *pix_fmts;
  const int            *supported_samplerates;
  const AVSampleFormat *sample_fmts;
  const uint64_t       *channel_layouts;
  uint8_t               max_lowres;
  const AVClass        *priv_class;

  // Actually, there is more here, but nothing more of the public API
};

typedef AVCodec_56 AVCodec_57;
typedef AVCodec_56 AVCodec_58;

enum AVChannelOrder
{
  AV_CHANNEL_ORDER_UNSPEC,
  AV_CHANNEL_ORDER_NATIVE,
  AV_CHANNEL_ORDER_CUSTOM,
  AV_CHANNEL_ORDER_AMBISONIC
};

enum AVChannel
{
  AV_CHAN_NONE = -1,
  AV_CHAN_FRONT_LEFT,
  AV_CHAN_FRONT_RIGHT,
  AV_CHAN_FRONT_CENTER,
  AV_CHAN_LOW_FREQUENCY,
  AV_CHAN_BACK_LEFT,
  AV_CHAN_BACK_RIGHT,
  AV_CHAN_FRONT_LEFT_OF_CENTER,
  AV_CHAN_FRONT_RIGHT_OF_CENTER,
  AV_CHAN_BACK_CENTER,
  AV_CHAN_SIDE_LEFT,
  AV_CHAN_SIDE_RIGHT,
  AV_CHAN_TOP_CENTER,
  AV_CHAN_TOP_FRONT_LEFT,
  AV_CHAN_TOP_FRONT_CENTER,
  AV_CHAN_TOP_FRONT_RIGHT,
  AV_CHAN_TOP_BACK_LEFT,
  AV_CHAN_TOP_BACK_CENTER,
  AV_CHAN_TOP_BACK_RIGHT,
  AV_CHAN_STEREO_LEFT = 29,
  AV_CHAN_STEREO_RIGHT,
  AV_CHAN_WIDE_LEFT,
  AV_CHAN_WIDE_RIGHT,
  AV_CHAN_SURROUND_DIRECT_LEFT,
  AV_CHAN_SURROUND_DIRECT_RIGHT,
  AV_CHAN_LOW_FREQUENCY_2,
  AV_CHAN_TOP_SIDE_LEFT,
  AV_CHAN_TOP_SIDE_RIGHT,
  AV_CHAN_BOTTOM_FRONT_CENTER,
  AV_CHAN_BOTTOM_FRONT_LEFT,
  AV_CHAN_BOTTOM_FRONT_RIGHT,
  AV_CHAN_SIDE_SURROUND_LEFT,  ///<  +90 degrees, Lss, SiL
  AV_CHAN_SIDE_SURROUND_RIGHT, ///<  -90 degrees, Rss, SiR
  AV_CHAN_TOP_SURROUND_LEFT,   ///< +110 degrees, Lvs, TpLS
  AV_CHAN_TOP_SURROUND_RIGHT,  ///< -110 degrees, Rvs, TpRS
  AV_CHAN_BINAURAL_LEFT = 61,
  AV_CHAN_BINAURAL_RIGHT,
  AV_CHAN_UNUSED         = 0x200,
  AV_CHAN_UNKNOWN        = 0x300,
  AV_CHAN_AMBISONIC_BASE = 0x400,
  AV_CHAN_AMBISONIC_END  = 0x7ff
};

struct AVChannelCustom
{
  AVChannel id{};
  char      name[16]{};
  void     *opaque{};
};

struct AVChannelLayout
{
  AVChannelOrder order{};
  int            nb_channels{};
  union
  {
    uint64_t         mask;
    AVChannelCustom *map;
  } u{};
  void *opaque{};
};

struct AVCodec_59
{
  const char           *name;
  const char           *long_name;
  AVMediaType           type;
  AVCodecID             id;
  int                   capabilities;
  uint8_t               max_lowres;
  const AVRational     *supported_framerates;
  const AVPixelFormat  *pix_fmts;
  const int            *supported_samplerates;
  const AVSampleFormat *sample_fmts;
  const uint64_t       *channel_layouts;
  const AVClass        *priv_class;

  // Actually, there is more here, but nothing more of the public API
};

struct AVCodec_60
{
  const char            *name;
  const char            *long_name;
  AVMediaType            type;
  AVCodecID              id;
  int                    capabilities;
  uint8_t                max_lowres;
  const AVRational      *supported_framerates;
  const AVPixelFormat   *pix_fmts;
  const int             *supported_samplerates;
  const AVSampleFormat  *sample_fmts;
  const uint64_t        *deprecated_channel_layouts; // Actually named channel_layouts
  const AVClass         *priv_class;
  const AVProfile       *profiles;
  const char            *wrapper_name;
  const AVChannelLayout *channel_layouts; // Actual name ch_layouts
};

struct AVCodec_61
{
  const char            *name;
  const char            *long_name;
  AVMediaType            type;
  AVCodecID              id;
  int                    capabilities;
  uint8_t                max_lowres;
  const AVRational      *supported_framerates;
  const AVPixelFormat   *pix_fmts;
  const int             *supported_samplerates;
  const AVSampleFormat  *sample_fmts;
  const AVClass         *priv_class;
  const AVProfile       *profiles;
  const char            *wrapper_name;
  const AVChannelLayout *channel_layouts;
};

typedef AVCodec_61 AVCodec_62;

} // namespace libffmpeg::internal::avcodec
