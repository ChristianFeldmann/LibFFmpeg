/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <AVFormat/wrappers/AVFormatContextWrapper.h>
#include <libHandling/IFFmpegLibraries.h>

namespace ffmpeg
{

class Demuxer
{
public:
  Demuxer()                = delete;
  Demuxer(const Demuxer &) = delete;
  Demuxer(Demuxer &&demuxer);
  Demuxer(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  bool openFile(const std::filesystem::path &path);

  avformat::AVFormatContextWrapper *getFormatContext() { return &this->formatContext; }

  std::optional<avcodec::AVPacketWrapper> getNextPacket();

private:
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  avformat::AVFormatContextWrapper formatContext;
};

} // namespace ffmpeg
