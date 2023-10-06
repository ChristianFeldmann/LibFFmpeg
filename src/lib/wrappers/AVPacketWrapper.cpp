/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVPacketWrapper.h"

namespace LibFFmpeg
{

AVPacketWrapper::AVPacketWrapper(AVPacket *packet, const LibraryVersions &libraryVersions)
{
  this->libraryVersions = libraryVersions;

  if (this->libraryVersions.avcodec.major == 56)
  {
    auto p    = reinterpret_cast<AVPacket_56 *>(packet);
    p->data   = nullptr;
    p->size   = 0;
    this->pkt = reinterpret_cast<AVPacket *>(p);
  }
  else if (this->libraryVersions.avcodec.major == 57 || //
           this->libraryVersions.avcodec.major == 58)
  {
    auto p    = reinterpret_cast<AVPacket_57_58 *>(packet);
    p->data   = nullptr;
    p->size   = 0;
    this->pkt = reinterpret_cast<AVPacket *>(p);
  }
  else if (this->libraryVersions.avcodec.major == 59 || //
           this->libraryVersions.avcodec.major == 60)
  {
    auto p    = reinterpret_cast<AVPacket_59_60 *>(packet);
    p->data   = nullptr;
    p->size   = 0;
    this->pkt = reinterpret_cast<AVPacket *>(p);
  }
  else
    throw std::runtime_error("Invalid library version");
}

void AVPacketWrapper::clear()
{
  this->pkt             = nullptr;
  this->libraryVersions = {};
}

void AVPacketWrapper::setData(const ByteVector &data)
{
  if (this->libraryVersions.avcodec.major == 56)
  {
    auto p           = reinterpret_cast<AVPacket_56 *>(this->pkt);
    this->packetData = data;
    p->data          = (uint8_t *)data.data();
    p->size          = static_cast<int>(data.size());
    this->data       = p->data;
    size             = p->size;
  }
  else if (this->libraryVersions.avcodec.major == 57 || //
           this->libraryVersions.avcodec.major == 58)
  {
    auto p           = reinterpret_cast<AVPacket_57_58 *>(this->pkt);
    this->packetData = data;
    p->data          = (uint8_t *)data.data();
    p->size          = static_cast<int>(data.size());
    this->data       = p->data;
    size             = p->size;
  }
  else if (this->libraryVersions.avcodec.major == 59 || //
           this->libraryVersions.avcodec.major == 60)
  {
    auto p           = reinterpret_cast<AVPacket_59_60 *>(this->pkt);
    this->packetData = data;
    p->data          = (uint8_t *)data.data();
    p->size          = static_cast<int>(data.size());
    this->data       = p->data;
    size             = p->size;
  }
  else
    throw std::runtime_error("Invalid library version");
}

void AVPacketWrapper::setPTS(int64_t pts)
{
  if (this->libraryVersions.avcodec.major == 56)
  {
    auto p    = reinterpret_cast<AVPacket_56 *>(this->pkt);
    p->pts    = pts;
    this->pts = pts;
  }
  else if (this->libraryVersions.avcodec.major == 57 || //
           this->libraryVersions.avcodec.major == 58)
  {
    auto p    = reinterpret_cast<AVPacket_57_58 *>(this->pkt);
    p->pts    = pts;
    this->pts = pts;
  }
  else if (this->libraryVersions.avcodec.major == 59 || //
           this->libraryVersions.avcodec.major == 60)
  {
    auto p    = reinterpret_cast<AVPacket_59_60 *>(this->pkt);
    p->pts    = pts;
    this->pts = pts;
  }
  else
    throw std::runtime_error("Invalid library version");
}

void AVPacketWrapper::setDTS(int64_t dts)
{
  if (this->libraryVersions.avcodec.major == 56)
  {
    auto p    = reinterpret_cast<AVPacket_56 *>(this->pkt);
    p->dts    = dts;
    this->dts = dts;
  }
  else if (this->libraryVersions.avcodec.major == 57 || //
           this->libraryVersions.avcodec.major == 58)
  {
    auto p    = reinterpret_cast<AVPacket_57_58 *>(this->pkt);
    p->dts    = dts;
    this->dts = dts;
  }
  else if (this->libraryVersions.avcodec.major == 59 || //
           this->libraryVersions.avcodec.major == 60)
  {
    auto p    = reinterpret_cast<AVPacket_59_60 *>(this->pkt);
    p->dts    = dts;
    this->dts = dts;
  }
  else
    throw std::runtime_error("Invalid library version");
}

AVPacket *AVPacketWrapper::getPacket()
{
  this->update();
  return this->pkt;
}

int AVPacketWrapper::getStreamIndex()
{
  this->update();
  return this->stream_index;
}

int64_t AVPacketWrapper::getPTS()
{
  this->update();
  return this->pts;
}

int64_t AVPacketWrapper::getDTS()
{
  this->update();
  return this->dts;
}

int64_t AVPacketWrapper::getDuration()
{
  this->update();
  return this->duration;
}

int AVPacketWrapper::getFlags()
{
  this->update();
  return this->flags;
}

bool AVPacketWrapper::getFlagKeyframe()
{
  this->update();
  return this->flags & AV_PKT_FLAG_KEY;
}

bool AVPacketWrapper::getFlagCorrupt()
{
  this->update();
  return this->flags & AV_PKT_FLAG_CORRUPT;
}

bool AVPacketWrapper::getFlagDiscard()
{
  this->update();
  return this->flags & AV_PKT_FLAG_DISCARD;
}

uint8_t *AVPacketWrapper::getData()
{
  this->update();
  return this->data;
}

int AVPacketWrapper::getDataSize()
{
  this->update();
  return this->size;
}

void AVPacketWrapper::update()
{
  if (this->pkt == nullptr)
    return;

  if (this->libraryVersions.avcodec.major == 56)
  {
    auto p = reinterpret_cast<AVPacket_56 *>(this->pkt);

    this->buf             = p->buf;
    this->pts             = p->pts;
    this->dts             = p->dts;
    this->data            = p->data;
    this->size            = p->size;
    this->stream_index    = p->stream_index;
    this->flags           = p->flags;
    this->side_data       = p->side_data;
    this->side_data_elems = p->side_data_elems;
    this->duration        = p->duration;
    this->pos             = p->pos;
  }
  else if (this->libraryVersions.avcodec.major == 57 || //
           this->libraryVersions.avcodec.major == 58)
  {
    auto p = reinterpret_cast<AVPacket_57_58 *>(this->pkt);

    this->buf             = p->buf;
    this->pts             = p->pts;
    this->dts             = p->dts;
    this->data            = p->data;
    this->size            = p->size;
    this->stream_index    = p->stream_index;
    this->flags           = p->flags;
    this->side_data       = p->side_data;
    this->side_data_elems = p->side_data_elems;
    this->duration        = p->duration;
    this->pos             = p->pos;
  }
  else if (this->libraryVersions.avcodec.major == 59 || //
           this->libraryVersions.avcodec.major == 60)
  {
    auto p = reinterpret_cast<AVPacket_59_60 *>(this->pkt);

    this->buf             = p->buf;
    this->pts             = p->pts;
    this->dts             = p->dts;
    this->data            = p->data;
    this->size            = p->size;
    this->stream_index    = p->stream_index;
    this->flags           = p->flags;
    this->side_data       = p->side_data;
    this->side_data_elems = p->side_data_elems;
    this->duration        = p->duration;
    this->pos             = p->pos;
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace LibFFmpeg
