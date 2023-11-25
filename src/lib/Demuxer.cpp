/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Demuxer.h"

namespace ffmpeg
{

Demuxer::Demuxer(std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : formatContext(avformat::AVFormatContextWrapper(librariesInterface))
{
  this->librariesInterface = librariesInterface;
}

Demuxer::Demuxer(Demuxer &&demuxer)
    : librariesInterface(std::move(demuxer.librariesInterface)),
      formatContext(std::move(demuxer.formatContext))
{
}

ResultAndLog Demuxer::openFile(const std::filesystem::path &path)
{
  Log log;

  if (!this->librariesInterface)
  {
    log.push_back("Error. Libraries not loaded");
    return {false, log};
  }

  const auto result = this->formatContext.openFile(path);

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

} // namespace ffmpeg
