/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace ffmpeg::internal::avformat
{

// AVFormatContext is part of avformat.
// These functions give us version independent access to the structs.
struct AVFormatContext_56
{
  const AVClass  *av_class;
  AVInputFormat  *iformat;
  AVOutputFormat *oformat;
  void           *priv_data;
  AVIOContext    *pb;
  int             ctx_flags;
  unsigned int    nb_streams; //
  AVStream      **streams;    //
  char            filename[1024];
  int64_t         start_time;
  int64_t         duration; //
  int             bit_rate;
  unsigned int    packet_size;
  int             max_delay;
  int             flags;
  unsigned int    probesize;
  int             max_analyze_duration;
  const uint8_t  *key;
  int             keylen;
  unsigned int    nb_programs;
  AVProgram     **programs;
  AVCodecID       video_codec_id;
  AVCodecID       audio_codec_id;
  AVCodecID       subtitle_codec_id;
  unsigned int    max_index_size;
  unsigned int    max_picture_buffer;
  unsigned int    nb_chapters;
  AVChapter     **chapters;
  AVDictionary   *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFormatContext_57
{
  const AVClass  *av_class;
  AVInputFormat  *iformat;
  AVOutputFormat *oformat;
  void           *priv_data;
  AVIOContext    *pb;
  int             ctx_flags;
  unsigned int    nb_streams;
  AVStream      **streams;
  char            filename[1024];
  int64_t         start_time;
  int64_t         duration;
  int64_t         bit_rate;
  unsigned int    packet_size;
  int             max_delay;
  int             flags;
  int64_t         probesize;
  int64_t         max_analyze_duration;
  const uint8_t  *key;
  int             keylen;
  unsigned int    nb_programs;
  AVProgram     **programs;
  AVCodecID       video_codec_id;
  AVCodecID       audio_codec_id;
  AVCodecID       subtitle_codec_id;
  unsigned int    max_index_size;
  unsigned int    max_picture_buffer;
  unsigned int    nb_chapters;
  AVChapter     **chapters;
  AVDictionary   *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFormatContext_58
{
  const AVClass  *av_class;
  AVInputFormat  *iformat;
  AVOutputFormat *oformat;
  void           *priv_data;
  AVIOContext    *pb;
  int             ctx_flags;
  unsigned int    nb_streams;
  AVStream      **streams;
  char            filename[1024];
  char           *url;
  int64_t         start_time;
  int64_t         duration;
  int64_t         bit_rate;
  unsigned int    packet_size;
  int             max_delay;
  int             flags;
  int64_t         probesize;
  int64_t         max_analyze_duration;
  const uint8_t  *key;
  int             keylen;
  unsigned int    nb_programs;
  AVProgram     **programs;
  AVCodecID       video_codec_id;
  AVCodecID       audio_codec_id;
  AVCodecID       subtitle_codec_id;
  unsigned int    max_index_size;
  unsigned int    max_picture_buffer;
  unsigned int    nb_chapters;
  AVChapter     **chapters;
  AVDictionary   *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFormatContext_59
{
  const AVClass  *av_class;
  AVInputFormat  *iformat;
  AVOutputFormat *oformat;
  void           *priv_data;
  AVIOContext    *pb;
  int             ctx_flags;
  unsigned int    nb_streams;
  AVStream      **streams;
  char           *url;
  int64_t         start_time;
  int64_t         duration;
  int64_t         bit_rate;
  unsigned int    packet_size;
  int             max_delay;
  int             flags;
  int64_t         probesize;
  int64_t         max_analyze_duration;
  const uint8_t  *key;
  int             keylen;
  unsigned int    nb_programs;
  AVProgram     **programs;
  AVCodecID       video_codec_id;
  AVCodecID       audio_codec_id;
  AVCodecID       subtitle_codec_id;
  unsigned int    max_index_size;
  unsigned int    max_picture_buffer;
  unsigned int    nb_chapters;
  AVChapter     **chapters;
  AVDictionary   *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVFormatContext_59 AVFormatContext_60;

struct AVFormatContext_61
{
  const AVClass  *av_class;
  AVInputFormat  *iformat;
  AVOutputFormat *oformat;
  void           *priv_data;
  AVIOContext    *pb;
  int             ctx_flags;
  unsigned int    nb_streams;
  AVStream      **streams;
  unsigned int    nb_stream_groups; // Added in 61
  AVStreamGroup **stream_groups;    // Added in 61
  unsigned int    nb_chapters;      // Moved in 61
  AVChapter     **chapters;         // Moved in 61
  char           *url;
  int64_t         start_time;
  int64_t         duration;
  int64_t         bit_rate;
  unsigned int    packet_size;
  int             max_delay;
  int             flags;
  int64_t         probesize;
  int64_t         max_analyze_duration;
  const uint8_t  *key;
  int             keylen;
  unsigned int    nb_programs;
  AVProgram     **programs;
  AVCodecID       video_codec_id;
  AVCodecID       audio_codec_id;
  AVCodecID       subtitle_codec_id;
  AVCodecID       data_codec_id; // Added in 61
  AVDictionary   *metadata;      // Moved in 61

  // Actually, there is more here, but the variables above are the only we need.
};

} // namespace ffmpeg::internal::avformat
