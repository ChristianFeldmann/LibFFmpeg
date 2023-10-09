/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVPacketWrapper.h"

#include <wrappers/Functions.h>

#include "CastCodecClasses.h"

namespace ffmpeg::avcodec
{

AVPacketWrapper::AVPacketWrapper(AVPacket *packet, const LibraryVersions &libraryVersions)
    : libraryVersions(libraryVersions), packet(packet)
{
}

void AVPacketWrapper::setData(const ByteVector &data)
{
  // Use av functions to create buffers
}

void AVPacketWrapper::setTimestamps(const int64_t dts, const int64_t pts)
{
  if (this->libraryVersions.avcodec.major == 56)
  {
    auto p = reinterpret_cast<AVPacket_56 *>(this->packet);
    p->dts = dts;
    p->pts = pts;
  }
  else if (this->libraryVersions.avcodec.major == 57 || //
           this->libraryVersions.avcodec.major == 58)
  {
    auto p = reinterpret_cast<AVPacket_57 *>(this->packet);
    p->dts = dts;
    p->pts = pts;
  }
  else if (this->libraryVersions.avcodec.major == 59 || //
           this->libraryVersions.avcodec.major == 60)
  {
    auto p = reinterpret_cast<AVPacket_59 *>(this->packet);
    p->dts = dts;
    p->pts = pts;
  }
  else
    throw std::runtime_error("Invalid library version");
}

int AVPacketWrapper::getStreamIndex() const
{
  int index;
  CAST_AVCODEC_GET_MEMBER(this->libraryVersions, AVPacket, this->packet, index, stream_index);
  return index;
}

int64_t AVPacketWrapper::getPTS() const
{
  int pts;
  CAST_AVCODEC_GET_MEMBER(this->libraryVersions, AVPacket, this->packet, pts, pts);
  return pts;
}

int64_t AVPacketWrapper::getDTS() const
{
  int dts;
  CAST_AVCODEC_GET_MEMBER(this->libraryVersions, AVPacket, this->packet, dts, dts);
  return dts;
}

int64_t AVPacketWrapper::getDuration() const
{
  int duration;
  CAST_AVCODEC_GET_MEMBER(this->libraryVersions, AVPacket, this->packet, duration, duration);
  return duration;
}

AVPacketWrapper::Flags AVPacketWrapper::getFlags() const
{
  int flagsAsInt;
  CAST_AVCODEC_GET_MEMBER(this->libraryVersions, AVPacket, this->packet, flagsAsInt, flags);

  Flags flags;

  if (flagsAsInt & AV_PKT_FLAG_KEY)
    flags.keyframe = true;
  if (flagsAsInt & AV_PKT_FLAG_CORRUPT)
    flags.corrupt = true;
  if (flagsAsInt & AV_PKT_FLAG_DISCARD)
    flags.discard = true;

  return flags;
}

ByteVector AVPacketWrapper::getData() const
{
  uint8_t *data;
  CAST_AVCODEC_GET_MEMBER(this->libraryVersions, AVPacket, this->packet, data, data);

  int dataSize;
  CAST_AVCODEC_GET_MEMBER(this->libraryVersions, AVPacket, this->packet, dataSize, size);

  return copyDataFromRawArray(data, dataSize);
}

} // namespace ffmpeg::avcodec
