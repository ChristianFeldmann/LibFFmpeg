/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Demuxer.h"

#include <common/Formatting.h>

namespace ffmpeg
{

namespace
{

std::string logPacket(const avcodec::AVPacketWrapper &packet)
{
  std::stringstream stream;
  stream << " DTS " << packet.getDTS();
  stream << " PTS ";
  const auto pts = packet.getPTS();
  if (pts)
    stream << *pts;
  else
    stream << "-";
  stream << " Flags [" << to_string(packet.getFlags()) << "]";
  return stream.str();
}

} // namespace

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

bool Demuxer::openFile(const std::filesystem::path &path)
{
  if (!this->ffmpegLibraries)
  {
    this->ffmpegLibraries->log(LogLevel::Error, "Libraries not loaded.");
    return false;
  }

  const auto result = this->formatContext.openFile(path);
  return result;
}

std::optional<avcodec::AVPacketWrapper> Demuxer::getNextPacket()
{
  avcodec::AVPacketWrapper packet(this->ffmpegLibraries);
  if (!this->formatContext.getNextPacket(packet))
  {
    this->ffmpegLibraries->log(LogLevel::Debug, "Got empty packet");
    return {};
  }
  this->ffmpegLibraries->log(LogLevel::Debug, "Got Packet with " + logPacket(packet));
  return packet;
}

} // namespace ffmpeg
