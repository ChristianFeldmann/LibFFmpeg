/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVStreamWrapper.h"

#include <wrappers/AVCodec/AVPacketWrapper.h>

#include "CastFormatClasses.h"

namespace ffmpeg::avformat
{

namespace
{

// AVStream is part of AVFormat
struct AVStream_56
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
};

struct AVProbeData_57
{
  const char    *filename;
  unsigned char *buf;
  int            buf_size;
  const char    *mime_type;
};

struct AVStream_57
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
};

struct AVStream_58
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
};

struct AVStream_59
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
};

struct AVStream_60
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
};

} // namespace

AVStreamWrapper::AVStreamWrapper(AVStream                                 *stream,
                                 std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : stream(stream), librariesInterface(librariesInterface)
{
}

AVMediaType AVStreamWrapper::getCodecType() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters.getCodecType();

  if (const auto codecContext = this->getCodecContext())
    return codecContext.getCodecType();

  return {AVMEDIA_TYPE_UNKNOWN};
}

std::string AVStreamWrapper::getCodecTypeName() const
{
  const auto codecParameters = this->getCodecParameters();
  if (!codecParameters)
    return {};

  switch (codecParameters.getCodecType())
  {
  case AVMEDIA_TYPE_UNKNOWN:
    return "Unknown";
  case AVMEDIA_TYPE_VIDEO:
    return "Video";
  case AVMEDIA_TYPE_AUDIO:
    return "Audio";
  case AVMEDIA_TYPE_DATA:
    return "Data";
  case AVMEDIA_TYPE_SUBTITLE:
    return "Subtitle";
  case AVMEDIA_TYPE_ATTACHMENT:
    return "Attachment";

  default:
    return {};
  }
}

AVCodecID AVStreamWrapper::getCodecID() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters.getCodecID();

  if (const auto codecContext = this->getCodecContext())
    return codecContext.getCodecID();

  return {AV_CODEC_ID_NONE};
}

avcodec::AVCodecDescriptorWrapper AVStreamWrapper::getCodecDescriptor() const
{
  const auto codecID = this->getCodecID();

  const auto descriptor = this->librariesInterface->avcodec.avcodec_descriptor_get(codecID);
  return avcodec::AVCodecDescriptorWrapper(descriptor);
}

AVRational AVStreamWrapper::getAverageFrameRate() const
{
  AVRational frameRate;
  CAST_AVFORMAT_GET_MEMBER(AVStream, this->stream, frameRate, avg_frame_rate);
  return frameRate;
}

AVRational AVStreamWrapper::getTimeBase() const
{
  AVRational timebase;
  CAST_AVFORMAT_GET_MEMBER(AVStream, this->stream, timebase, time_base);

  if (timebase.den != 0 && timebase.num != 0)
    return timebase;

  // The stream time_base seems not to be set. Try the time_base in the codec.
  if (const auto codecContext = this->getCodecContext())
    return codecContext.getTimeBase();

  return {};
}

Size AVStreamWrapper::getFrameSize() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters.getSize();

  if (const auto codecContext = this->getCodecContext())
    return codecContext.getSize();

  return {};
}

ColorSpace AVStreamWrapper::getColorspace() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters.getColorspace();

  if (const auto codecContext = this->getCodecContext())
    return codecContext.getColorspace();

  return {};
}

avutil::AVPixFmtDescriptorWrapper AVStreamWrapper::getPixelFormat() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters.getPixelFormat();

  if (const auto codecContext = this->getCodecContext())
    return codecContext.getPixelFormat();

  return {};
}

ByteVector AVStreamWrapper::getExtradata() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters.getExtradata();

  if (const auto codecContext = this->getCodecContext())
    return codecContext.getExtradata();

  return {};
}

int AVStreamWrapper::getIndex() const
{
  int index;
  CAST_AVFORMAT_GET_MEMBER(AVStream, this->stream, index, index);
  return index;
}

avcodec::AVCodecContextWrapper AVStreamWrapper::getCodecContext() const
{
  const auto version = this->librariesInterface->getLibrariesVersion().avformat.major;
  if (version == 56)
  {
    const auto p = reinterpret_cast<AVStream_56 *>(this->stream);
    return avcodec::AVCodecContextWrapper(p->codec, this->librariesInterface);
  }
  if (version == 57)
  {
    const auto p = reinterpret_cast<AVStream_57 *>(this->stream);
    return avcodec::AVCodecContextWrapper(p->codec, this->librariesInterface);
  }
  if (version == 58)
  {
    const auto p = reinterpret_cast<AVStream_58 *>(this->stream);
    return avcodec::AVCodecContextWrapper(p->codec, this->librariesInterface);
  }

  return {};
};

AVCodecParametersWrapper AVStreamWrapper::getCodecParameters() const
{
  const auto version = this->librariesInterface->getLibrariesVersion().avformat.major;
  if (version == 57)
  {
    const auto p = reinterpret_cast<AVStream_57 *>(this->stream);
    return AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }
  if (version == 58)
  {
    const auto p = reinterpret_cast<AVStream_58 *>(this->stream);
    return AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }
  if (version == 59)
  {
    const auto p = reinterpret_cast<AVStream_59 *>(this->stream);
    return AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }
  if (version == 60)
  {
    const auto p = reinterpret_cast<AVStream_60 *>(this->stream);
    return AVCodecParametersWrapper(p->codecpar, this->librariesInterface);
  }

  return {};
}

} // namespace ffmpeg::avformat
