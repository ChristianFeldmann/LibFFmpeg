/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace libffmpeg::internal::avutil
{

struct AVFrame_54
{
  uint8_t *     data[AV_NUM_DATA_POINTERS];
  int           linesize[AV_NUM_DATA_POINTERS];
  uint8_t **    extended_data;
  int           width, height;
  int           nb_samples;
  int           format;
  int           key_frame;
  AVPictureType pict_type;
  uint8_t *     base[AV_NUM_DATA_POINTERS];
  AVRational    sample_aspect_ratio;
  int64_t       pts;
  int64_t       pkt_pts;
  int64_t       pkt_dts;
  int           coded_picture_number;   // Deprecated
  int           display_picture_number; // Deprecated
  int           quality;
  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFrame_55
{
  uint8_t *     data[AV_NUM_DATA_POINTERS];
  int           linesize[AV_NUM_DATA_POINTERS];
  uint8_t **    extended_data;
  int           width, height;
  int           nb_samples;
  int           format;
  int           key_frame;
  AVPictureType pict_type;
  AVRational    sample_aspect_ratio;
  int64_t       pts;
  int64_t       pkt_pts;
  int64_t       pkt_dts;
  int           coded_picture_number;   // Deprecated
  int           display_picture_number; // Deprecated
  int           quality;
  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVFrame_55 AVFrame_56;

struct AVFrame_57
{
  uint8_t *                     data[AV_NUM_DATA_POINTERS];
  int                           linesize[AV_NUM_DATA_POINTERS];
  uint8_t **                    extended_data;
  int                           width, height;
  int                           nb_samples;
  int                           format;
  int                           key_frame;
  AVPictureType                 pict_type;
  AVRational                    sample_aspect_ratio;
  int64_t                       pts;
  int64_t                       pkt_dts;
  AVRational                    time_base;
  int                           coded_picture_number;   // Deprecated
  int                           display_picture_number; // Deprecated
  int                           quality;
  void *                        opaque;
  int                           repeat_pict;
  int                           interlaced_frame;
  int                           top_field_first;
  int                           palette_has_changed;
  int64_t                       reordered_opaque;
  int                           sample_rate;
  uint64_t                      channel_layout;
  AVBufferRef *                 buf[AV_NUM_DATA_POINTERS];
  AVBufferRef **                extended_buf;
  int                           nb_extended_buf;
  AVFrameSideData **            side_data;
  int                           nb_side_data;
  int                           flags;
  AVColorRange                  color_range;
  AVColorPrimaries              color_primaries;
  AVColorTransferCharacteristic color_trc;
  AVColorSpace                  colorspace;
  AVChromaLocation              chroma_location;
  int64_t                       best_effort_timestamp;
  int64_t                       pkt_pos;
  int64_t                       pkt_duration;
  AVDictionary *                metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVFrame_57 AVFrame_58;

struct AVFrame_59
{
  uint8_t *                     data[AV_NUM_DATA_POINTERS];
  int                           linesize[AV_NUM_DATA_POINTERS];
  uint8_t **                    extended_data;
  int                           width, height;
  int                           nb_samples;
  int                           format;
  int                           key_frame; // Deprecated
  AVPictureType                 pict_type;
  AVRational                    sample_aspect_ratio;
  int64_t                       pts;
  int64_t                       pkt_dts;
  AVRational                    time_base;
  int                           quality;
  void *                        opaque;
  int                           repeat_pict;
  int                           interlaced_frame;    // Deprecated
  int                           top_field_first;     // Deprecated
  int                           palette_has_changed; // Deprecated
  int64_t                       reordered_opaque;
  int                           sample_rate;
  AVBufferRef *                 buf[AV_NUM_DATA_POINTERS];
  AVBufferRef **                extended_buf;
  int                           nb_extended_buf;
  AVFrameSideData **            side_data;
  int                           nb_side_data;
  int                           flags;
  AVColorRange                  color_range;
  AVColorPrimaries              color_primaries;
  AVColorTransferCharacteristic color_trc;
  AVColorSpace                  colorspace;
  AVChromaLocation              chroma_location;
  int64_t                       best_effort_timestamp;
  int64_t                       pkt_pos; // Deprecated
  AVDictionary *                metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

} // namespace libffmpeg::internal::avutil
