/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

namespace libffmpeg::internal::avcodec
{

/* In avcodec version 56, this struct does not exist yet.
 * This is a dummy class that is just here so that we can use the generic
 * CAST_AVFORMAT_GET_MEMBER and CAST_AVFORMAT_SET_MEMBER macros. In reality,
 * we will never do the cast to this struct.
 */
struct AVCodecParameters_56
{
  AVMediaType  codec_type;
  AVCodecID    codec_id;
  uint8_t *    extradata;
  int          extradata_size;
  int          format;
  int          profile;
  int          level;
  int          width;
  int          height;
  AVRational   sample_aspect_ratio;
  AVColorSpace color_space;
};

struct AVCodecParameters_57
{
  AVMediaType                   codec_type;
  AVCodecID                     codec_id;
  uint32_t                      codec_tag;
  uint8_t *                     extradata;
  int                           extradata_size;
  int                           format;
  int64_t                       bit_rate;
  int                           bits_per_coded_sample;
  int                           bits_per_raw_sample;
  int                           profile;
  int                           level;
  int                           width;
  int                           height;
  AVRational                    sample_aspect_ratio;
  AVFieldOrder                  field_order;
  AVColorRange                  color_range;
  AVColorPrimaries              color_primaries;
  AVColorTransferCharacteristic color_trc;
  AVColorSpace                  color_space;
  AVChromaLocation              chroma_location;
  int                           video_delay;

  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVCodecParameters_57 AVCodecParameters_58;
typedef AVCodecParameters_57 AVCodecParameters_59;
typedef AVCodecParameters_57 AVCodecParameters_60;

struct AVCodecParameters_61
{
  AVMediaType                   codec_type;
  AVCodecID                     codec_id;
  uint32_t                      codec_tag;
  uint8_t *                     extradata;
  int                           extradata_size;
  AVPacketSideData *            coded_side_data;
  int                           nb_coded_side_data;
  int                           format;
  int64_t                       bit_rate;
  int                           bits_per_coded_sample;
  int                           bits_per_raw_sample;
  int                           profile;
  int                           level;
  int                           width;
  int                           height;
  AVRational                    sample_aspect_ratio;
  AVRational                    framerate;
  AVFieldOrder                  field_order;
  AVColorRange                  color_range;
  AVColorPrimaries              color_primaries;
  AVColorTransferCharacteristic color_trc;
  AVColorSpace                  color_space;
  AVChromaLocation              chroma_location;
  int                           video_delay;

  // Actually, there is more here, but the variables above are the only we need.
};

} // namespace libffmpeg::internal::avcodec
