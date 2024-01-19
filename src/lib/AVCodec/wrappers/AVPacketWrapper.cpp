/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVPacketWrapper.h"

#include <common/Functions.h>

#include "AVPacketWrapperInternal.h"
#include "CastCodecClasses.h"

#include <cstring>

namespace ffmpeg::avcodec
{

namespace
{

using ffmpeg::internal::AVPacket;
using ffmpeg::internal::avcodec::AVPacket_56;
using ffmpeg::internal::avcodec::AVPacket_57;
using ffmpeg::internal::avcodec::AVPacket_58;
using ffmpeg::internal::avcodec::AVPacket_59;
using ffmpeg::internal::avcodec::AVPacket_60;

constexpr auto AV_PKT_FLAG_KEY     = 0x0001; ///< The packet contains a keyframe
constexpr auto AV_PKT_FLAG_CORRUPT = 0x0002; ///< The packet content is corrupted
constexpr auto AV_PKT_FLAG_DISCARD = 0x0004; ///< Not required for output and should be discarded

} // namespace

AVPacketWrapper::AVPacketWrapper(AVPacketWrapper &&other)
{
  this->packet          = other.packet;
  other.packet          = nullptr;
  this->ffmpegLibraries = std::move(other.ffmpegLibraries);
}

AVPacketWrapper &AVPacketWrapper::operator=(AVPacketWrapper &&other)
{
  this->packet          = other.packet;
  other.packet          = nullptr;
  this->ffmpegLibraries = std::move(other.ffmpegLibraries);
  return *this;
}

AVPacketWrapper::AVPacketWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : ffmpegLibraries(ffmpegLibraries)
{
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
  if (ffmpegLibraries->getLibrariesVersion().avcodec.major == 56)
  {
    auto newPacket  = new AVPacket_56;
    newPacket->data = nullptr;
    newPacket->size = 0;
    this->packet    = reinterpret_cast<AVPacket *>(newPacket);
    this->ffmpegLibraries->avcodec.av_init_packet(this->packet);
  }
  else
    this->packet = this->ffmpegLibraries->avcodec.av_packet_alloc();
  if (this->packet == nullptr)
    throw std::runtime_error("Unable to allocate new AVPacket");
}

AVPacketWrapper::AVPacketWrapper(const ByteVector                 &data,
                                 std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : ffmpegLibraries(ffmpegLibraries)
{
  if (ffmpegLibraries->getLibrariesVersion().avcodec.major == 56)
  {
    this->packet = reinterpret_cast<AVPacket *>(new AVPacket_56);
    this->ffmpegLibraries->avcodec.av_init_packet(this->packet);
  }
  else
    this->packet = this->ffmpegLibraries->avcodec.av_packet_alloc();
  if (this->packet == nullptr)
    throw std::runtime_error("Unable to allocate new AVPacket");

  const auto ret =
      this->ffmpegLibraries->avcodec.av_new_packet(this->packet, static_cast<int>(data.size()));
  if (ret > 0)
    throw std::runtime_error("Error calling av_new_packet");

  uint8_t *dataPointer;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, dataPointer, data);
  std::memcpy(dataPointer, data.data(), data.size());
}

AVPacketWrapper::~AVPacketWrapper()
{
  if (!this->packet)
    return;

  if (this->ffmpegLibraries->getLibrariesVersion().avcodec.major == 56)
  {
    this->ffmpegLibraries->avcodec.av_free_packet(this->packet);
    auto castPointer = reinterpret_cast<AVPacket_56 *>(this->packet);
    delete castPointer;
    this->packet = nullptr;
  }
  else
    this->ffmpegLibraries->avcodec.av_packet_free(&this->packet);
}

std::optional<AVPacketWrapper> AVPacketWrapper::clone() const
{
  if (this->packet == nullptr)
    return {};

  AVPacketWrapper clonedWrapper(this->ffmpegLibraries);
  clonedWrapper.ffmpegLibraries = this->ffmpegLibraries;

  const auto ret =
      this->ffmpegLibraries->avcodec.av_copy_packet(clonedWrapper.getPacket(), this->packet);
  if (ret < 0)
    return {};

  return clonedWrapper;
}

void AVPacketWrapper::setTimestamps(const int64_t dts, const int64_t pts)
{
  CAST_AVCODEC_SET_MEMBER(AVPacket, this->packet, dts, dts);
  CAST_AVCODEC_SET_MEMBER(AVPacket, this->packet, pts, pts);
}

int AVPacketWrapper::getStreamIndex() const
{
  int index;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, index, stream_index);
  return index;
}

std::optional<int64_t> AVPacketWrapper::getPTS() const
{
  int64_t pts;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, pts, pts);

  constexpr int64_t AV_NOPTS_VALUE = 0x8000000000000000;
  if (pts == AV_NOPTS_VALUE)
    return {};
  return pts;
}

int64_t AVPacketWrapper::getDTS() const
{
  int64_t dts;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, dts, dts);
  return dts;
}

int64_t AVPacketWrapper::getDuration() const
{
  int64_t duration;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, duration, duration);
  return duration;
}

AVPacketWrapper::Flags AVPacketWrapper::getFlags() const
{
  int flagsAsInt;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, flagsAsInt, flags);

  Flags flags;

  if (flagsAsInt & AV_PKT_FLAG_KEY)
    flags.keyframe = true;
  if (flagsAsInt & AV_PKT_FLAG_CORRUPT)
    flags.corrupt = true;
  if (flagsAsInt & AV_PKT_FLAG_DISCARD)
    flags.discard = true;

  return flags;
}

int AVPacketWrapper::getDataSize() const
{
  int dataSize;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, dataSize, size);
  return dataSize;
}

ByteVector AVPacketWrapper::getData() const
{
  uint8_t *data{};
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, data, data);

  int dataSize{};
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, dataSize, size);

  return copyDataFromRawArray(data, dataSize);
}

} // namespace ffmpeg::avcodec
