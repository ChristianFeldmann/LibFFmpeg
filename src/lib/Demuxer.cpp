/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Demuxer.h"

namespace ffmpeg
{

Demuxer::Demuxer(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : formatContext(avformat::AVFormatContextWrapper(ffmpegLibraries))
{
  this->ffmpegLibraries = ffmpegLibraries;
}

Demuxer::Demuxer(Demuxer &&demuxer)
    : ffmpegLibraries(std::move(demuxer.ffmpegLibraries)),
      formatContext(std::move(demuxer.formatContext))
{
}

ResultAndLog Demuxer::openFile(const std::filesystem::path &path)
{
  Log log;

  if (!this->ffmpegLibraries)
  {
    log.push_back("Error. Libraries not loaded");
    return {false, log};
  }

  const auto result = this->formatContext.openFile(path);

  return result;
}

std::optional<avcodec::AVPacketWrapper> Demuxer::getNextPacket()
{
  avcodec::AVPacketWrapper packet(this->ffmpegLibraries);
  packet.allocatePacket();
  if (!this->formatContext.getNextPacket(packet))
    return {};
  return packet;
}

} // namespace ffmpeg
