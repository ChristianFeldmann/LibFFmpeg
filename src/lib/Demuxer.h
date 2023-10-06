/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVFormatContextWrapper.h>
#include <wrappers/AVPacketWrapper.h>

namespace LibFFmpeg
{

class Demuxer
{
public:
  Demuxer(std::shared_ptr<FFmpegLibrariesInterface> libraries);

  ResultAndLog openFile(std::filesystem::path path);

  LibFFmpeg::AVFormatContextWrapper getFormatContext() const { return this->formatContext; }

  AVPacketWrapper getNextPacket();

private:
  std::shared_ptr<FFmpegLibrariesInterface> libraries;

  LibFFmpeg::AVFormatContextWrapper formatContext;
};

} // namespace LibFFmpeg
