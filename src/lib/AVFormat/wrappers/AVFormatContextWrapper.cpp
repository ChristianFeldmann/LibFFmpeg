/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFormatContextWrapper.h"
#include "AVFormatContextWrapperInternal.h"
#include "AVStreamWrapper.h"

#include "CastFormatClasses.h"

namespace libffmpeg::avformat
{

using libffmpeg::internal::AVDictionary;
using libffmpeg::internal::AVInputFormat;
using libffmpeg::internal::AVStream;
using libffmpeg::internal::avformat::AVFormatContext_56;
using libffmpeg::internal::avformat::AVFormatContext_57;
using libffmpeg::internal::avformat::AVFormatContext_58;
using libffmpeg::internal::avformat::AVFormatContext_59;
using libffmpeg::internal::avformat::AVFormatContext_60;
using libffmpeg::internal::avformat::AVFormatContext_61;

AVFormatContextWrapper::AVFormatContextWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
{
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
  this->ffmpegLibraries = ffmpegLibraries;
}

AVFormatContextWrapper::AVFormatContextWrapper(AVFormatContextWrapper &&wrapper)
{
  this->formatContext   = wrapper.formatContext;
  wrapper.formatContext = nullptr;
  this->ffmpegLibraries = std::move(wrapper.ffmpegLibraries);
}

AVFormatContextWrapper::~AVFormatContextWrapper()
{
  if (this->formatContext)
    this->ffmpegLibraries->avformat.avformat_close_input(&this->formatContext);
}

bool AVFormatContextWrapper::openFile(const std::filesystem::path path)
{
  if (this->formatContext)
  {
    this->ffmpegLibraries->log(LogLevel::Error,
                               "Error opening file. AVFormatContext is already initialized.");
    return false;
  }

  return this->openInputAndFindStreamInfo(path);
}

bool AVFormatContextWrapper::openInput(std::unique_ptr<avformat::AVIOInputContext> ioInput)
{
  if (this->formatContext)
  {
    this->ffmpegLibraries->log(LogLevel::Error,
                               "Error opening file. AVFormatContext is already initialized.");
    return false;
  }

  if (!ioInput || !(*ioInput))
  {
    this->ffmpegLibraries->log(LogLevel::Error, "Invalid ioInput given.");
    return false;
  }

  this->formatContext = this->ffmpegLibraries->avformat.avformat_alloc_context();
  if (this->formatContext == nullptr)
  {
    this->ffmpegLibraries->log(LogLevel::Error, "Error allocating AVFormatContext.");
    return false;
  }

  this->ioInput = std::move(ioInput);

  CAST_AVFORMAT_SET_MEMBER(
      AVFormatContext, this->formatContext, pb, this->ioInput->getAVIOContext());

  return this->openInputAndFindStreamInfo({});
}

AVFormatContextWrapper::operator bool() const
{
  return this->formatContext != nullptr;
};

std::vector<AVStreamWrapper> AVFormatContextWrapper::getStreams() const
{
  std::vector<AVStreamWrapper> streams;
  for (int i = 0; i < this->getNumberStreams(); ++i)
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
  const auto returnCode = toReturnCode(
      this->ffmpegLibraries->avformat.av_read_frame(this->formatContext, packet.getPacket()));

  if (returnCode != ReturnCode::Ok)
    this->ffmpegLibraries->log(LogLevel::Error,
                               "Error getting next packet (av_read_frame). Return code " +
                                   ReturnCodeMapper.getName(returnCode));

  return returnCode == ReturnCode::Ok;
}

bool AVFormatContextWrapper::openInputAndFindStreamInfo(
    const std::optional<std::filesystem::path> path)
{
  auto returnCode = toReturnCode(this->ffmpegLibraries->avformat.avformat_open_input(
      &this->formatContext, path ? path->string().c_str() : nullptr, nullptr, nullptr));
  if (returnCode != ReturnCode::Ok)
  {
    this->ffmpegLibraries->log(LogLevel::Error,
                               "Error opening file (avformat_open_input). Return code " +
                                   ReturnCodeMapper.getName(returnCode));
    return false;
  }

  returnCode = toReturnCode(
      this->ffmpegLibraries->avformat.avformat_find_stream_info(this->formatContext, nullptr));
  if (returnCode != ReturnCode::Ok)
  {
    this->ffmpegLibraries->log(LogLevel::Error,
                               "Error opening file (avformat_open_input). Return code " +
                                   ReturnCodeMapper.getName(returnCode));
    return false;
  }

  return true;
}

} // namespace libffmpeg::avformat
