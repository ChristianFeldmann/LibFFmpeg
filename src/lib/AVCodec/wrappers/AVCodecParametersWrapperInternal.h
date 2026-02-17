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

/* In avcodec version 56, this struct does not exist yet.
 * This is a dummy class that is just here so that we can use the generic
 * CAST_AVFORMAT_GET_MEMBER and CAST_AVFORMAT_SET_MEMBER macros. In reality,
 * we will never do the cast to this struct.
 */
struct AVCodecParameters_56
{
  AVMediaType  codec_type;
  AVCodecID    codec_id;
  uint8_t     *extradata;
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
  uint8_t                      *extradata;
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
  uint64_t                      channel_layout;
  int                           channels;
  int                           sample_rate;
  int                           block_align;
  int                           frame_size;
  int                           initial_padding;
  int                           trailing_padding;
  int                           seek_preroll;
};

typedef AVCodecParameters_57 AVCodecParameters_58;

struct AVCodecParameters_59
{
  AVMediaType                   codec_type;
  AVCodecID                     codec_id;
  uint32_t                      codec_tag;
  uint8_t                      *extradata;
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
  uint64_t                      channel_layout;
  int                           channels;
  int                           sample_rate;
  int                           block_align;
  int                           frame_size;
  int                           initial_padding;
  int                           trailing_padding;
  int                           seek_preroll;
  AVChannelLayout               ch_layout;
};

struct AVCodecParameters_60
{
  AVMediaType                   codec_type;
  AVCodecID                     codec_id;
  uint32_t                      codec_tag;
  uint8_t                      *extradata;
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
  uint64_t                      channel_layout;
  int                           channels;
  int                           sample_rate;
  int                           block_align;
  int                           frame_size;
  int                           initial_padding;
  int                           trailing_padding;
  int                           seek_preroll;
  AVChannelLayout               ch_layout;
  AVRational                    framerate;
  AVPacketSideData             *coded_side_data;
  int                           nb_coded_side_data;
};

struct AVCodecParameters_61
{
  AVMediaType                   codec_type;
  AVCodecID                     codec_id;
  uint32_t                      codec_tag;
  uint8_t                      *extradata;
  int                           extradata_size;
  AVPacketSideData             *coded_side_data;
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
  AVChannelLayout               ch_layout;
  int                           sample_rate;
  int                           block_align;
  int                           frame_size;
  int                           initial_padding;
  int                           trailing_padding;
  int                           seek_preroll;
};

typedef AVCodecParameters_61 AVCodecParameters_62;

} // namespace libffmpeg::internal::avcodec
