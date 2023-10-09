/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVStreamWrapper.h"

#include <wrappers/AVCodec/AVPacketWrapper.h>

namespace ffmpeg::avformat
{

namespace
{

// AVStream is part of AVFormat
typedef struct AVStream_56
{
  int                  index;
  int                  id;
  AVCodecContext      *codec;
  void                *priv_data;
  struct AVFrac        pts;
  AVRational           time_base;
  int64_t              start_time;
  int64_t              duration;
  int64_t              nb_frames;
  int                  disposition;
  enum AVDiscard       discard;
  AVRational           sample_aspect_ratio;
  AVDictionary        *metadata;
  AVRational           avg_frame_rate;
  avcodec::AVPacket_56 attached_pic;
  AVPacketSideData    *side_data;
  int                  nb_side_data;
  int                  event_flags;
} AVStream_56;

typedef struct AVProbeData_57
{
  const char    *filename;
  unsigned char *buf;
  int            buf_size;
  const char    *mime_type;
} AVProbeData_57;

typedef struct AVStream_57
{
  int                  index;
  int                  id;
  AVCodecContext      *codec; // Deprecated. Might be removed in the next major version.
  void                *priv_data;
  struct AVFrac        pts; // Deprecated. Might be removed in the next major version.
  AVRational           time_base;
  int64_t              start_time;
  int64_t              duration;
  int64_t              nb_frames;
  int                  disposition;
  enum AVDiscard       discard;
  AVRational           sample_aspect_ratio;
  AVDictionary        *metadata;
  AVRational           avg_frame_rate;
  avcodec::AVPacket_57 attached_pic;
  AVPacketSideData    *side_data;
  int                  nb_side_data;
  int                  event_flags;
  // All field following this line are not part of the public API and may change/be removed.
  // However, we still need them here because further below some fields which are part of the public
  // API will follow. I really don't understand who thought up this idiotic scheme...
#define MAX_STD_TIMEBASES (30 * 12 + 30 + 3 + 6)
  struct
  {
    int64_t last_dts;
    int64_t duration_gcd;
    int     duration_count;
    int64_t rfps_duration_sum;
    double (*duration_error)[2][MAX_STD_TIMEBASES];
    int64_t codec_info_duration;
    int64_t codec_info_duration_fields;
    int     found_decoder;
    int64_t last_duration;
    int64_t fps_first_dts;
    int     fps_first_dts_idx;
    int64_t fps_last_dts;
    int     fps_last_dts_idx;
  } * info;
  int                          pts_wrap_bits;
  int64_t                      first_dts;
  int64_t                      cur_dts;
  int64_t                      last_IP_pts;
  int                          last_IP_duration;
  int                          probe_packets;
  int                          codec_info_nb_frames;
  enum AVStreamParseType       need_parsing;
  struct AVCodecParserContext *parser;
  struct AVPacketList         *last_in_packet_buffer;
  AVProbeData_57               probe_data;
#define MAX_REORDER_DELAY 16
  int64_t       pts_buffer[MAX_REORDER_DELAY + 1];
  AVIndexEntry *index_entries;
  int           nb_index_entries;
  unsigned int  index_entries_allocated_size;
  AVRational    r_frame_rate;
  int           stream_identifier;
  int64_t       interleaver_chunk_size;
  int64_t       interleaver_chunk_duration;
  int           request_probe;
  int           skip_to_keyframe;
  int           skip_samples;
  int64_t       start_skip_samples;
  int64_t       first_discard_sample;
  int64_t       last_discard_sample;
  int           nb_decoded_frames;
  int64_t       mux_ts_offset;
  int64_t       pts_wrap_reference;
  int           pts_wrap_behavior;
  int           update_initial_durations_done;
  int64_t       pts_reorder_error[MAX_REORDER_DELAY + 1];
  uint8_t       pts_reorder_error_count[MAX_REORDER_DELAY + 1];
  int64_t       last_dts_for_order_check;
  uint8_t       dts_ordered;
  uint8_t       dts_misordered;
  int           inject_global_side_data;
  // All fields above this line are not part of the public API.
  // All fields below are part of the public API and ABI again.
  char              *recommended_encoder_configuration;
  AVRational         display_aspect_ratio;
  struct FFFrac     *priv_pts;
  AVStreamInternal  *internal;
  AVCodecParameters *codecpar;
} AVStream_57;

typedef struct AVStream_58
{
  int                  index;
  int                  id;
  AVCodecContext      *codec;
  void                *priv_data;
  AVRational           time_base;
  int64_t              start_time;
  int64_t              duration;
  int64_t              nb_frames;
  int                  disposition;
  enum AVDiscard       discard;
  AVRational           sample_aspect_ratio;
  AVDictionary        *metadata;
  AVRational           avg_frame_rate;
  avcodec::AVPacket_58 attached_pic;
  AVPacketSideData    *side_data;
  int                  nb_side_data;
  int                  event_flags;
  AVRational           r_frame_rate;
  char                *recommended_encoder_configuration;
  AVCodecParameters   *codecpar;

  // All field following this line are not part of the public API and may change/be removed.
} AVStream_58;

typedef struct AVStream_59
{
  int                  index;
  int                  id;
  void                *priv_data;
  AVRational           time_base;
  int64_t              start_time;
  int64_t              duration;
  int64_t              nb_frames;
  int                  disposition;
  enum AVDiscard       discard;
  AVRational           sample_aspect_ratio;
  AVDictionary        *metadata;
  AVRational           avg_frame_rate;
  avcodec::AVPacket_59 attached_pic;
  AVPacketSideData    *side_data;
  int                  nb_side_data;
  int                  event_flags;
  AVRational           r_frame_rate;
  AVCodecParameters   *codecpar;
  int                  pts_wrap_bits;
} AVStream_59;

typedef struct AVStream_60
{
  const AVClass       *av_class;
  int                  index;
  int                  id;
  AVCodecParameters   *codecpar;
  void                *priv_data;
  AVRational           time_base;
  int64_t              start_time;
  int64_t              duration;
  int64_t              nb_frames;
  int                  disposition;
  enum AVDiscard       discard;
  AVRational           sample_aspect_ratio;
  AVDictionary        *metadata;
  AVRational           avg_frame_rate;
  avcodec::AVPacket_60 attached_pic;
  AVPacketSideData    *side_data;
  int                  nb_side_data;
  int                  event_flags;
  AVRational           r_frame_rate;
  int                  pts_wrap_bits;
} AVStream_60;

} // namespace

AVStreamWrapper::AVStreamWrapper(AVStream                                 *avStream,
                                 std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
{
  this->stream             = avStream;
  this->librariesInterface = librariesInterface;
  this->update();
}

AVMediaType AVStreamWrapper::getCodecType()
{
  this->update();
  if (this->librariesInterface->getLibrariesVersion().avformat.major <= 56 || !this->codecpar)
    return this->codec.getCodecType();
  return this->codecpar.getCodecType();
}

std::string AVStreamWrapper::getCodecTypeName()
{
  auto type = this->codecpar.getCodecType();

  switch (type)
  {
  case -1:
    return "Unknown";
  case 0:
    return "Video";
  case 1:
    return "Audio";
  case 2:
    return "Data";
  case 3:
    return "Subtitle";
  case 4:
    return "Attachment";

  default:
    return {};
  }
}

AVCodecID AVStreamWrapper::getCodecID()
{
  this->update();
  if (this->stream == nullptr)
    return AV_CODEC_ID_NONE;

  if (this->librariesInterface->getLibrariesVersion().avformat.major <= 56 || !this->codecpar)
    return this->codec.getCodecID();
  else
    return this->codecpar.getCodecID();
}

avcodec::AVCodecContextWrapper &AVStreamWrapper::getCodec()
{
  this->update();
  return this->codec;
};

AVRational AVStreamWrapper::getAvgFrameRate()
{
  this->update();
  return this->avg_frame_rate;
}

AVRational AVStreamWrapper::getTimeBase()
{
  this->update();
  if (this->time_base.den == 0 || this->time_base.num == 0)
    // The stream time_base seems not to be set. Try the time_base in the codec.
    return this->codec.getTimeBase();
  return this->time_base;
}

Size AVStreamWrapper::getFrameSize()
{
  this->update();
  if (this->librariesInterface->getLibrariesVersion().avformat.major <= 56 || !this->codecpar)
    return this->codec.getSize();
  return this->codecpar.getSize();
}

AVColorSpace AVStreamWrapper::getColorspace()
{
  this->update();
  if (this->librariesInterface->getLibrariesVersion().avformat.major <= 56 || !this->codecpar)
    return this->codec.getColorspace();
  return this->codecpar.getColorspace();
}

AVPixelFormat AVStreamWrapper::getPixelFormat()
{
  this->update();
  if (this->librariesInterface->getLibrariesVersion().avformat.major <= 56 || !this->codecpar)
    return this->codec.getPixelFormat();
  return this->codecpar.getPixelFormat();
}

ByteVector AVStreamWrapper::getExtradata()
{
  this->update();
  if (this->librariesInterface->getLibrariesVersion().avformat.major <= 56 || !this->codecpar)
    return this->codec.getExtradata();
  return this->codecpar.getExtradata();
}

int AVStreamWrapper::getIndex()
{
  this->update();
  return this->index;
}

AVCodecParametersWrapper AVStreamWrapper::getCodecpar()
{
  this->update();
  return this->codecpar;
}

void AVStreamWrapper::update()
{
  if (this->stream == nullptr)
    return;

  // Copy values from the source pointer
  if (this->librariesInterface->getLibrariesVersion().avformat.major == 56)
  {
    auto p                    = reinterpret_cast<AVStream_56 *>(this->stream);
    this->index               = p->index;
    this->id                  = p->id;
    this->codec               = avcodec::AVCodecContextWrapper(p->codec, this->librariesInterface);
    this->time_base           = p->time_base;
    this->start_time          = p->start_time;
    this->duration            = p->duration;
    this->nb_frames           = p->nb_frames;
    this->disposition         = p->nb_frames;
    this->discard             = p->discard;
    this->sample_aspect_ratio = p->sample_aspect_ratio;
    this->avg_frame_rate      = p->avg_frame_rate;
    this->nb_side_data        = p->nb_side_data;
    this->event_flags         = p->event_flags;
  }
  else if (this->librariesInterface->getLibrariesVersion().avformat.major == 57)
  {
    auto p                    = reinterpret_cast<AVStream_57 *>(this->stream);
    this->index               = p->index;
    this->id                  = p->id;
    this->codec               = avcodec::AVCodecContextWrapper(p->codec, this->librariesInterface);
    this->time_base           = p->time_base;
    this->start_time          = p->start_time;
    this->duration            = p->duration;
    this->nb_frames           = p->nb_frames;
    this->disposition         = p->nb_frames;
    this->discard             = p->discard;
    this->sample_aspect_ratio = p->sample_aspect_ratio;
    this->avg_frame_rate      = p->avg_frame_rate;
    this->nb_side_data        = p->nb_side_data;
    this->event_flags         = p->event_flags;
    this->codecpar            = AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }
  else if (this->librariesInterface->getLibrariesVersion().avformat.major == 58)
  {
    auto p                    = reinterpret_cast<AVStream_58 *>(this->stream);
    this->index               = p->index;
    this->id                  = p->id;
    this->codec               = avcodec::AVCodecContextWrapper(p->codec, this->librariesInterface);
    this->time_base           = p->time_base;
    this->start_time          = p->start_time;
    this->duration            = p->duration;
    this->nb_frames           = p->nb_frames;
    this->disposition         = p->nb_frames;
    this->discard             = p->discard;
    this->sample_aspect_ratio = p->sample_aspect_ratio;
    this->avg_frame_rate      = p->avg_frame_rate;
    this->nb_side_data        = p->nb_side_data;
    this->event_flags         = p->event_flags;
    this->codecpar            = AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }
  else if (this->librariesInterface->getLibrariesVersion().avformat.major == 59)
  {
    auto p                    = reinterpret_cast<AVStream_59 *>(this->stream);
    this->index               = p->index;
    this->id                  = p->id;
    this->time_base           = p->time_base;
    this->start_time          = p->start_time;
    this->duration            = p->duration;
    this->nb_frames           = p->nb_frames;
    this->disposition         = p->nb_frames;
    this->discard             = p->discard;
    this->sample_aspect_ratio = p->sample_aspect_ratio;
    this->avg_frame_rate      = p->avg_frame_rate;
    this->nb_side_data        = p->nb_side_data;
    this->event_flags         = p->event_flags;
    this->codecpar            = AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }
  else if (this->librariesInterface->getLibrariesVersion().avformat.major == 60)
  {
    auto p                    = reinterpret_cast<AVStream_60 *>(this->stream);
    this->index               = p->index;
    this->id                  = p->id;
    this->time_base           = p->time_base;
    this->start_time          = p->start_time;
    this->duration            = p->duration;
    this->nb_frames           = p->nb_frames;
    this->disposition         = p->nb_frames;
    this->discard             = p->discard;
    this->sample_aspect_ratio = p->sample_aspect_ratio;
    this->avg_frame_rate      = p->avg_frame_rate;
    this->nb_side_data        = p->nb_side_data;
    this->event_flags         = p->event_flags;
    this->codecpar            = AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace ffmpeg::avformat
