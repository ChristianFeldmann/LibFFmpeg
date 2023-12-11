/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/IFFmpegLibraries.h>
#include <wrappers/AVCodec/AVPacketWrapper.h>
#include <wrappers/AVFormat/AVFormatContextWrapper.h>

namespace ffmpeg
{

class Demuxer
{
public:
  Demuxer()                = delete;
  Demuxer(const Demuxer &) = delete;
  Demuxer(Demuxer &&demuxer);
  Demuxer(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  ResultAndLog openFile(const std::filesystem::path &path);

  avformat::AVFormatContextWrapper *getFormatContext() { return &this->formatContext; }

  std::optional<avcodec::AVPacketWrapper> getNextPacket();

private:
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  avformat::AVFormatContextWrapper formatContext;
};

} // namespace ffmpeg
