/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFormatContextWrapper.h"
#include "AVStreamWrapper.h"

#include "CastFormatClasses.h"

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVChapter;
using ffmpeg::internal::AVClass;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVInputFormat;
using ffmpeg::internal::AVIOContext;
using ffmpeg::internal::AVProgram;
using ffmpeg::internal::AVStream;

// AVFormatContext is part of avformat.
// These functions give us version independent access to the structs.
struct AVFormatContext_56
{
  const AVClass         *av_class;
  struct AVInputFormat  *iformat;
  struct AVOutputFormat *oformat;
  void                  *priv_data;
  AVIOContext           *pb;
  int                    ctx_flags;
  unsigned int           nb_streams; //
  AVStream             **streams;    //
  char                   filename[1024];
  int64_t                start_time;
  int64_t                duration; //
  int                    bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  unsigned int           probesize;
  int                    max_analyze_duration;
  const uint8_t         *key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram            **programs;
  AVCodecID              video_codec_id;
  AVCodecID              audio_codec_id;
  AVCodecID              subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter            **chapters;
  AVDictionary          *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFormatContext_57
{
  const AVClass         *av_class;
  struct AVInputFormat  *iformat;
  struct AVOutputFormat *oformat;
  void                  *priv_data;
  AVIOContext           *pb;
  int                    ctx_flags;
  unsigned int           nb_streams;
  AVStream             **streams;
  char                   filename[1024];
  int64_t                start_time;
  int64_t                duration;
  int64_t                bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  unsigned int           probesize;
  int                    max_analyze_duration;
  const uint8_t         *key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram            **programs;
  AVCodecID              video_codec_id;
  AVCodecID              audio_codec_id;
  AVCodecID              subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter            **chapters;
  AVDictionary          *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFormatContext_58
{
  const AVClass         *av_class;
  struct AVInputFormat  *iformat;
  struct AVOutputFormat *oformat;
  void                  *priv_data;
  AVIOContext           *pb;
  int                    ctx_flags;
  unsigned int           nb_streams;
  AVStream             **streams;
  char                   filename[1024];
  char                  *url;
  int64_t                start_time;
  int64_t                duration;
  int64_t                bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  int64_t                probesize;
  int64_t                max_analyze_duration;
  const uint8_t         *key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram            **programs;
  AVCodecID              video_codec_id;
  AVCodecID              audio_codec_id;
  AVCodecID              subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter            **chapters;
  AVDictionary          *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFormatContext_59
{
  const AVClass         *av_class;
  struct AVInputFormat  *iformat;
  struct AVOutputFormat *oformat;
  void                  *priv_data;
  AVIOContext           *pb;
  int                    ctx_flags;
  unsigned int           nb_streams;
  AVStream             **streams;
  char                  *url;
  int64_t                start_time;
  int64_t                duration;
  int64_t                bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  int64_t                probesize;
  int64_t                max_analyze_duration;
  const uint8_t         *key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram            **programs;
  AVCodecID              video_codec_id;
  AVCodecID              audio_codec_id;
  AVCodecID              subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter            **chapters;
  AVDictionary          *metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVFormatContext_59 AVFormatContext_60;

} // namespace

AVFormatContextWrapper::AVFormatContextWrapper(
    std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
{
  this->librariesInterface = librariesInterface;
}

AVFormatContextWrapper::AVFormatContextWrapper(AVFormatContextWrapper &&wrapper)
{
  this->formatContext      = std::move(wrapper.formatContext);
  this->librariesInterface = std::move(wrapper.librariesInterface);
}

ResultAndLog AVFormatContextWrapper::openFile(std::filesystem::path path)
{
  Log log;

  if (this->formatContext)
  {
    log.push_back("Error opening file. AVFormatContext is already initialized.");
    return {false, log};
  }

  auto ret = this->librariesInterface->avformat.avformat_open_input(
      &this->formatContext, path.string().c_str(), nullptr, nullptr);
  if (ret < 0)
  {
    log.push_back("Error opening file (avformat_open_input). Return code " + std::to_string(ret));
    return {false, log};
  }
  if (this->librariesInterface == nullptr)
  {
    log.push_back("Error opening file (avformat_open_input). Nullptr returned.");
    return {false, log};
  }

  ret = this->librariesInterface->avformat.avformat_find_stream_info(this->formatContext, nullptr);
  if (ret < 0)
  {
    log.push_back("Error opening file (avformat_open_input). Return code " + std::to_string(ret));
    return {false, log};
  }

  return {true, log};
}

AVFormatContextWrapper::operator bool() const
{
  return this->formatContext != nullptr;
};

std::vector<AVStreamWrapper> AVFormatContextWrapper::getStreams() const
{
  unsigned numberStreams{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, numberStreams, nb_streams);

  std::vector<AVStreamWrapper> streams;

  for (unsigned i = 0; i < numberStreams; ++i)
  {
    AVStream *streamPointer{};
    CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, streamPointer, streams[i]);
    streams.push_back(AVStreamWrapper(streamPointer, this->librariesInterface));
  }

  return streams;
}

int AVFormatContextWrapper::getNumberStreams() const
{
  unsigned numberStreams{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, numberStreams, nb_streams);
  return static_cast<int>(numberStreams);
}

AVStreamWrapper AVFormatContextWrapper::getStream(int idx) const
{
  AVStream *streamPointer{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, streamPointer, streams[idx]);
  return AVStreamWrapper(streamPointer, this->librariesInterface);
}

AVInputFormatWrapper AVFormatContextWrapper::getInputFormat() const
{
  AVInputFormat *inputFormatPointer{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, inputFormatPointer, iformat);
  return AVInputFormatWrapper(inputFormatPointer, this->librariesInterface);
}

int64_t AVFormatContextWrapper::getStartTime() const
{
  int64_t startTime{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, startTime, start_time);
  return startTime;
}

int64_t AVFormatContextWrapper::getDuration() const
{
  int64_t duration{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, duration, duration);
  return duration;
}

avutil::AVDictionaryWrapper AVFormatContextWrapper::getMetadata() const
{
  AVDictionary *metadata{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, metadata, metadata);
  return avutil::AVDictionaryWrapper(metadata, this->librariesInterface);
}

bool AVFormatContextWrapper::getNextPacket(avcodec::AVPacketWrapper &packet)
{
  const auto ret =
      this->librariesInterface->avformat.av_read_frame(this->formatContext, packet.getPacket());
  return ret == 0;
}

} // namespace ffmpeg::avformat
