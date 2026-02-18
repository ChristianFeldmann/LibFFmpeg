/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVPacketWrapperInternal.h>
#include <common/InternalTypes.h>

namespace libffmpeg::internal::avformat
{

class AVIndexEntry;
class AVStreamInternal;

// AVStream is part of AVFormat
struct AVStream_56
{
  int                  index{};
  int                  id{};
  AVCodecContext *     codec{};
  void *               priv_data{};
  AVFrac               pts{};
  AVRational           time_base{};
  int64_t              start_time{};
  int64_t              duration{};
  int64_t              nb_frames{};
  int                  disposition{};
  AVDiscard            discard{};
  AVRational           sample_aspect_ratio{};
  AVDictionary *       metadata{};
  AVRational           avg_frame_rate{};
  avcodec::AVPacket_56 attached_pic{};
  AVPacketSideData *   side_data{};
  int                  nb_side_data{};
  int                  event_flags{};
};

struct AVProbeData_57
{
  const char *   filename{};
  unsigned char *buf{};
  int            buf_size{};
  const char *   mime_type{};
};

struct AVStream_57
{
  int                  index{};
  int                  id{};
  AVCodecContext *     codec{}; // Deprecated.
  void *               priv_data{};
  AVFrac               pts{}; // Deprecated.
  AVRational           time_base{};
  int64_t              start_time{};
  int64_t              duration{};
  int64_t              nb_frames{};
  int                  disposition{};
  AVDiscard            discard{};
  AVRational           sample_aspect_ratio{};
  AVDictionary *       metadata{};
  AVRational           avg_frame_rate{};
  avcodec::AVPacket_57 attached_pic{};
  AVPacketSideData *   side_data{};
  int                  nb_side_data{};
  int                  event_flags{};
  // All field following this line are not part of the public API and may change/be removed.
  // However, we still need them here because further below some fields which are part of the public
  // API will follow. I really don't understand who thought up this idiotic scheme...
#define MAX_STD_TIMEBASES (30 * 12 + 30 + 3 + 6)
  struct
  {
    int64_t last_dts{};
    int64_t duration_gcd{};
    int     duration_count{};
    int64_t rfps_duration_sum{};
    double (*duration_error)[2][MAX_STD_TIMEBASES]{};
    int64_t codec_info_duration{};
    int64_t codec_info_duration_fields{};
    int     found_decoder{};
    int64_t last_duration{};
    int64_t fps_first_dts{};
    int     fps_first_dts_idx{};
    int64_t fps_last_dts{};
    int     fps_last_dts_idx{};
  } * info;
  int                          pts_wrap_bits{};
  int64_t                      first_dts{};
  int64_t                      cur_dts{};
  int64_t                      last_IP_pts{};
  int                          last_IP_duration{};
  int                          probe_packets{};
  int                          codec_info_nb_frames{};
  AVStreamParseType            need_parsing{};
  struct AVCodecParserContext *parser{};
  struct AVPacketList *        last_in_packet_buffer{};
  AVProbeData_57               probe_data{};
#define MAX_REORDER_DELAY 16
  int64_t       pts_buffer[MAX_REORDER_DELAY + 1]{};
  AVIndexEntry *index_entries{};
  int           nb_index_entries{};
  unsigned int  index_entries_allocated_size{};
  AVRational    r_frame_rate{};
  int           stream_identifier{};
  int64_t       interleaver_chunk_size{};
  int64_t       interleaver_chunk_duration{};
  int           request_probe{};
  int           skip_to_keyframe{};
  int           skip_samples{};
  int64_t       start_skip_samples{};
  int64_t       first_discard_sample{};
  int64_t       last_discard_sample{};
  int           nb_decoded_frames{};
  int64_t       mux_ts_offset{};
  int64_t       pts_wrap_reference{};
  int           pts_wrap_behavior{};
  int           update_initial_durations_done{};
  int64_t       pts_reorder_error[MAX_REORDER_DELAY + 1]{};
  uint8_t       pts_reorder_error_count[MAX_REORDER_DELAY + 1]{};
  int64_t       last_dts_for_order_check{};
  uint8_t       dts_ordered{};
  uint8_t       dts_misordered{};
  int           inject_global_side_data{};
  // All fields above this line are not part of the public API.
  // All fields below are part of the public API and ABI again.
  char *             recommended_encoder_configuration{};
  AVRational         display_aspect_ratio{};
  struct FFFrac *    priv_pts{};
  AVStreamInternal * internal{};
  AVCodecParameters *codecpar{};
};

struct AVStream_58
{
  int                  index{};
  int                  id{};
  AVCodecContext *     codec{};
  void *               priv_data{};
  AVRational           time_base{};
  int64_t              start_time{};
  int64_t              duration{};
  int64_t              nb_frames{};
  int                  disposition{};
  AVDiscard            discard{};
  AVRational           sample_aspect_ratio{};
  AVDictionary *       metadata{};
  AVRational           avg_frame_rate{};
  avcodec::AVPacket_58 attached_pic{};
  AVPacketSideData *   side_data{};
  int                  nb_side_data{};
  int                  event_flags{};
  AVRational           r_frame_rate{};
  char *               recommended_encoder_configuration{};
  AVCodecParameters *  codecpar{};

  // All field following this line are not part of the public API and may change/be removed.
};

struct AVStream_59
{
  int                  index{};
  int                  id{};
  void *               priv_data{};
  AVRational           time_base{};
  int64_t              start_time{};
  int64_t              duration{};
  int64_t              nb_frames{};
  int                  disposition{};
  AVDiscard            discard{};
  AVRational           sample_aspect_ratio{};
  AVDictionary *       metadata{};
  AVRational           avg_frame_rate{};
  avcodec::AVPacket_59 attached_pic{};
  AVPacketSideData *   side_data{};
  int                  nb_side_data{};
  int                  event_flags{};
  AVRational           r_frame_rate{};
  AVCodecParameters *  codecpar{};
  int                  pts_wrap_bits{};
};

struct AVStream_60
{
  const AVClass *      av_class{};
  int                  index{};
  int                  id{};
  AVCodecParameters *  codecpar{};
  void *               priv_data{};
  AVRational           time_base{};
  int64_t              start_time{};
  int64_t              duration{};
  int64_t              nb_frames{};
  int                  disposition{};
  AVDiscard            discard{};
  AVRational           sample_aspect_ratio{};
  AVDictionary *       metadata{};
  AVRational           avg_frame_rate{};
  avcodec::AVPacket_60 attached_pic{};
  AVPacketSideData *   side_data{};
  int                  nb_side_data{};
  int                  event_flags{};
  AVRational           r_frame_rate{};
  int                  pts_wrap_bits{};
};

typedef AVStream_60 AVStream_61;

struct AVStream_62
{
  const AVClass *      av_class{};
  int                  index{};
  int                  id{};
  AVCodecParameters *  codecpar{};
  void *               priv_data{};
  AVRational           time_base{};
  int64_t              start_time{};
  int64_t              duration{};
  int64_t              nb_frames{};
  int                  disposition{};
  AVDiscard            discard{};
  AVRational           sample_aspect_ratio{};
  AVDictionary *       metadata{};
  AVRational           avg_frame_rate{};
  avcodec::AVPacket_62 attached_pic{};
  int                  event_flags{};
  AVRational           r_frame_rate{};
  int                  pts_wrap_bits{};
};

} // namespace libffmpeg::internal::avformat
