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
  Demuxer(std::shared_ptr<FFmpegLibrariesInterface> libraries);

  ResultAndLog openFile(std::filesystem::path path);

  avformat::AVFormatContextWrapper getFormatContext() const { return this->formatContext; }

  bool getNextPacket(avcodec::AVPacketWrapper &packet);

private:
  std::shared_ptr<FFmpegLibrariesInterface> libraries;

  avformat::AVFormatContextWrapper formatContext;
};

} // namespace ffmpeg
