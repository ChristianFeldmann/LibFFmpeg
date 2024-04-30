/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace libffmpeg::avcodec
{

class AVPacketWrapper
{
public:
  AVPacketWrapper()                         = delete;
  AVPacketWrapper(AVPacketWrapper &&packet) = default;
  AVPacketWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  AVPacketWrapper(const ByteVector &data, std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  // Todo: Needs to be tested
  std::optional<AVPacketWrapper> clone() const;

  void setTimestamps(const int64_t dts, const int64_t pts);

  libffmpeg::internal::AVPacket *getPacket() const { return this->packet.get(); }

  struct Flags
  {
    bool keyframe{};
    bool corrupt{};
    bool discard{};
  };

  int                    getStreamIndex() const;
  std::optional<int64_t> getPTS() const;
  int64_t                getDTS() const;
  int64_t                getDuration() const;
  Flags                  getFlags() const;
  int                    getDataSize() const;
  ByteVector             getData() const;

  explicit operator bool() const { return this->packet != nullptr; };

private:
  void allocateNewPacket();

  class AVPacketDeleter
  {
  public:
    AVPacketDeleter() = default;
    AVPacketDeleter(const std::shared_ptr<IFFmpegLibraries> &ffmpegLibraries)
        : ffmpegLibraries(ffmpegLibraries){};
    void operator()(libffmpeg::internal::AVPacket *packet) const noexcept;

  private:
    std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
  };

  std::unique_ptr<libffmpeg::internal::AVPacket, AVPacketDeleter> packet{nullptr,
                                                                         AVPacketDeleter()};
  std::shared_ptr<IFFmpegLibraries>                               ffmpegLibraries{};
};

} // namespace libffmpeg::avcodec
