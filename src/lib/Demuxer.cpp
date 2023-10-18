/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Demuxer.h"

namespace ffmpeg
{

Demuxer::Demuxer(std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
{
  this->librariesInterface = librariesInterface;
}

ResultAndLog Demuxer::openFile(const std::filesystem::path &path)
{
  Log log;

  if (!this->librariesInterface)
  {
    log.push_back("Error. Libraries not loaded");
    return {false, log};
  }

  this->formatContext = ffmpeg::avformat::AVFormatContextWrapper(this->librariesInterface);
  const auto result   = this->formatContext.openFile(path);

  return result;
}

avcodec::AVPacketWrapper Demuxer::getNextPacket()
{
  avcodec::AVPacketWrapper packet(this->librariesInterface);
  packet.allocatePacket();
  if (!this->formatContext.getNextPacket(packet))
    return {};
  return packet;
}

avcodec::AVPacketWrapper Demuxer::getNextPacket(avcodec::AVPacketWrapper &&packet)
{
  if (!this->formatContext.getNextPacket(packet))
    return {};
  return std::move(packet);
}

} // namespace ffmpeg
