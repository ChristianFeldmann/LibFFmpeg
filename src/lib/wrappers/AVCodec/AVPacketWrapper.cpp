/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVPacketWrapper.h"

#include <wrappers/Functions.h>

#include "CastCodecClasses.h"

#include <cstring>

namespace ffmpeg::avcodec
{

AVPacketWrapper::AVPacketWrapper(AVPacket                                 *packet,
                                 std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : packet(packet), librariesInterface(librariesInterface)
{
}

AVPacketWrapper::AVPacketWrapper(std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : librariesInterface(librariesInterface)
{
}

AVPacketWrapper::AVPacketWrapper(const ByteVector                         &data,
                                 std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : librariesInterface(librariesInterface)
{
  this->packet = this->librariesInterface->avcodec.av_packet_alloc();
  if (this->packet == nullptr)
    throw std::runtime_error("Unable to allocate new AVPacket");

  const auto ret = this->librariesInterface->avcodec.av_new_packet(this->packet, data.size());

  uint8_t *dataPointer;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, dataPointer, data);
  std::memcpy(dataPointer, data.data(), data.size());
}

void AVPacketWrapper::allocatePacket()
{
  if (this->packet != nullptr)
    throw std::runtime_error("Packet already allocated");
  this->packet = this->librariesInterface->avcodec.av_packet_alloc();
  if (this->packet == nullptr)
    throw std::runtime_error("Unable to allocate new AVPacket");
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

int64_t AVPacketWrapper::getPTS() const
{
  int pts;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, pts, pts);
  return pts;
}

int64_t AVPacketWrapper::getDTS() const
{
  int dts;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, dts, dts);
  return dts;
}

int64_t AVPacketWrapper::getDuration() const
{
  int duration;
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
  uint8_t *data;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, data, data);

  int dataSize;
  CAST_AVCODEC_GET_MEMBER(AVPacket, this->packet, dataSize, size);

  return copyDataFromRawArray(data, dataSize);
}

} // namespace ffmpeg::avcodec
