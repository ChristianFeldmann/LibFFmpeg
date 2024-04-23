/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <AVFormat/wrappers/AVInputFormatWrapper.h>
#include <AVFormat/wrappers/AVStreamWrapper.h>
#include <AVUtil/wrappers/AVDictionaryWrapper.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace ffmpeg::avformat
{

class AVFormatContextWrapper
{
public:
  AVFormatContextWrapper()                               = delete;
  AVFormatContextWrapper(const AVFormatContextWrapper &) = delete;
  AVFormatContextWrapper(AVFormatContextWrapper &&wrapper);
  AVFormatContextWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  ~AVFormatContextWrapper();

  bool openFile(const Path path);

  explicit operator bool() const;

  std::vector<AVStreamWrapper> getStreams() const;
  int                          getNumberStreams() const;
  AVStreamWrapper              getStream(int idx) const;
  AVInputFormatWrapper         getInputFormat() const;
  int64_t                      getStartTime() const;
  int64_t                      getDuration() const;
  avutil::AVDictionaryWrapper  getMetadata() const;

  bool getNextPacket(avcodec::AVPacketWrapper &packet);

private:
  ffmpeg::internal::AVFormatContext *formatContext{nullptr};
  std::shared_ptr<IFFmpegLibraries>  ffmpegLibraries{};
};

} // namespace ffmpeg::avformat
