/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVCodec/AVPacketWrapper.h>
#include <wrappers/AVFormat/AVInputFormatWrapper.h>
#include <wrappers/AVFormat/AVStreamWrapper.h>
#include <wrappers/AVUtil/AVDictionaryWrapper.h>

#include <memory>

namespace ffmpeg::avformat
{

class AVFormatContextWrapper
{
public:
  AVFormatContextWrapper() = default;
  AVFormatContextWrapper(std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  ResultAndLog openFile(std::filesystem::path path);

  explicit operator bool() const;

  std::vector<AVStreamWrapper> getStreams() const;
  int                          getNumberStreams() const;
  AVStreamWrapper              getStream(int idx) const;
  AVInputFormatWrapper         getInputFormat() const;
  int64_t                      getStartTime() const;
  int64_t                      getDuration() const;
  avutil::AVDictionaryWrapper  getMetadata() const;

private:
  AVFormatContext                          *formatContext{nullptr};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avformat
