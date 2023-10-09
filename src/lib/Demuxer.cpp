/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Demuxer.h"

namespace ffmpeg
{

Demuxer::Demuxer(std::shared_ptr<FFmpegLibrariesInterface> libraries)
{
  this->libraries = libraries;
}

ResultAndLog Demuxer::openFile(std::filesystem::path path)
{
  Log log;

  if (!this->libraries)
  {
    log.push_back("Error. Libraries not loaded");
    return {false, log};
  }

  this->formatContext = ffmpeg::avformat::AVFormatContextWrapper(this->libraries);
  const auto result   = this->formatContext.openFile(path);

  return result;
}

avcodec::AVPacketWrapper Demuxer::getNextPacket()
{
  return {};
}

} // namespace ffmpeg
