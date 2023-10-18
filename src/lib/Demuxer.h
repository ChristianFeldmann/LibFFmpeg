/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVCodec/AVPacketWrapper.h>
#include <wrappers/AVFormat/AVFormatContextWrapper.h>

namespace ffmpeg
{

class Demuxer
{
public:
  Demuxer(std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  ResultAndLog openFile(const std::filesystem::path &path);

  avformat::AVFormatContextWrapper getFormatContext() const { return this->formatContext; }

  /* Read the next packet from the bitstream and return it. If there are no more packets,
   * the returned packet will be empty (bool(packet) == false). The overloaded function
   * allows you to move a packet into the function which will be reused. This saves the allocation
   * of a new packet.
   */
  avcodec::AVPacketWrapper getNextPacket();
  avcodec::AVPacketWrapper getNextPacket(avcodec::AVPacketWrapper &&packet);

private:
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface;

  avformat::AVFormatContextWrapper formatContext;
};

} // namespace ffmpeg
