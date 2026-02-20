/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Demuxer.h"

#include <common/Formatting.h>

namespace libffmpeg
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
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");

  this->ffmpegLibraries = ffmpegLibraries;
}

Demuxer::Demuxer(Demuxer &&demuxer) noexcept
    : ffmpegLibraries(std::move(demuxer.ffmpegLibraries)),
      formatContext(std::move(demuxer.formatContext))
{
}

Demuxer &Demuxer::operator=(Demuxer &&demuxer) noexcept
{
  if (this != &demuxer)
  {
    this->ffmpegLibraries = std::move(demuxer.ffmpegLibraries);
    this->formatContext   = std::move(demuxer.formatContext);
  }
  return *this;
}

bool Demuxer::openFile(const Path &path)
{
  return this->formatContext.openFile(path);
}

bool Demuxer::openInput(std::unique_ptr<avformat::AVIOInputContext> ioInput)
{
  return this->formatContext.openInput(std::move(ioInput));
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

} // namespace libffmpeg
