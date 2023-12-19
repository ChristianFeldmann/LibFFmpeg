/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFormatContextWrapper.h"
#include "AVFormatContextWrapperInternal.h"
#include "AVStreamWrapper.h"

#include "CastFormatClasses.h"

namespace ffmpeg::avformat
{

using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVInputFormat;
using ffmpeg::internal::AVStream;
using ffmpeg::internal::avformat::AVFormatContext_56;
using ffmpeg::internal::avformat::AVFormatContext_57;
using ffmpeg::internal::avformat::AVFormatContext_58;
using ffmpeg::internal::avformat::AVFormatContext_59;
using ffmpeg::internal::avformat::AVFormatContext_60;

AVFormatContextWrapper::AVFormatContextWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
{
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
  this->ffmpegLibraries = ffmpegLibraries;
}

AVFormatContextWrapper::AVFormatContextWrapper(AVFormatContextWrapper &&wrapper)
{
  this->formatContext   = std::move(wrapper.formatContext);
  this->ffmpegLibraries = std::move(wrapper.ffmpegLibraries);
}

AVFormatContextWrapper::~AVFormatContextWrapper()
{
  if (this->formatContext)
    this->ffmpegLibraries->avformat.avformat_close_input(&this->formatContext);
}

ResultAndLog AVFormatContextWrapper::openFile(std::filesystem::path path)
{
  Log log;

  if (this->formatContext)
  {
    log.push_back("Error opening file. AVFormatContext is already initialized.");
    return {false, log};
  }

  auto ret = this->ffmpegLibraries->avformat.avformat_open_input(
      &this->formatContext, path.string().c_str(), nullptr, nullptr);
  if (ret < 0)
  {
    log.push_back("Error opening file (avformat_open_input). Return code " + std::to_string(ret));
    return {false, log};
  }

  ret = this->ffmpegLibraries->avformat.avformat_find_stream_info(this->formatContext, nullptr);
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
  std::vector<AVStreamWrapper> streams;
  for (unsigned i = 0; i < this->getNumberStreams(); ++i)
    streams.push_back(this->getStream(i));
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
  if (idx < 0 || idx >= this->getNumberStreams())
    throw std::runtime_error("Invalid stream index");

  AVStream *streamPointer{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, streamPointer, streams[idx]);
  return AVStreamWrapper(streamPointer, this->ffmpegLibraries);
}

AVInputFormatWrapper AVFormatContextWrapper::getInputFormat() const
{
  AVInputFormat *inputFormatPointer{};
  CAST_AVFORMAT_GET_MEMBER(AVFormatContext, this->formatContext, inputFormatPointer, iformat);
  return AVInputFormatWrapper(inputFormatPointer, this->ffmpegLibraries);
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
  return avutil::AVDictionaryWrapper(metadata, this->ffmpegLibraries);
}

bool AVFormatContextWrapper::getNextPacket(avcodec::AVPacketWrapper &packet)
{
  const auto ret =
      this->ffmpegLibraries->avformat.av_read_frame(this->formatContext, packet.getPacket());
  return ret == 0;
}

} // namespace ffmpeg::avformat
