/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <AVFormat/wrappers/AVIOContextWrapper.h>
#include <AVFormat/wrappers/AVInputFormatWrapper.h>
#include <AVFormat/wrappers/AVStreamWrapper.h>
#include <AVUtil/wrappers/AVDictionaryWrapper.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace libffmpeg::avformat
{

class AVFormatContextWrapper
{
public:
  AVFormatContextWrapper()                                          = delete;
  AVFormatContextWrapper(const AVFormatContextWrapper &)            = delete;
  AVFormatContextWrapper &operator=(const AVFormatContextWrapper &) = delete;
  AVFormatContextWrapper(AVFormatContextWrapper &&wrapper) noexcept;
  AVFormatContextWrapper &operator=(AVFormatContextWrapper &&) noexcept;
  AVFormatContextWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  ~AVFormatContextWrapper();

  bool openFile(const Path path);
  bool openInput(std::unique_ptr<avformat::AVIOInputContext> ioInput);

  explicit operator bool() const;

  [[nodiscard]] std::vector<AVStreamWrapper> getStreams() const;
  [[nodiscard]] int                          getNumberStreams() const;
  [[nodiscard]] AVStreamWrapper              getStream(int idx) const;
  [[nodiscard]] AVInputFormatWrapper         getInputFormat() const;
  [[nodiscard]] int64_t                      getStartTime() const;
  [[nodiscard]] int64_t                      getDuration() const;
  [[nodiscard]] avutil::AVDictionaryWrapper  getMetadata() const;

  bool getNextPacket(avcodec::AVPacketWrapper &packet);

private:
  bool openInputAndFindStreamInfo(const std::optional<std::filesystem::path> path);

  libffmpeg::internal::AVFormatContext       *formatContext{nullptr};
  std::shared_ptr<IFFmpegLibraries>           ffmpegLibraries{};
  std::unique_ptr<avformat::AVIOInputContext> ioInput{};
};

} // namespace libffmpeg::avformat
