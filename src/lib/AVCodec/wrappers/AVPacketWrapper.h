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
  AVPacketWrapper()                                       = delete;
  AVPacketWrapper(const AVPacketWrapper &)                = delete;
  AVPacketWrapper &operator=(const AVPacketWrapper &)     = delete;
  AVPacketWrapper(AVPacketWrapper &&packet) noexcept      = default;
  AVPacketWrapper &operator=(AVPacketWrapper &&) noexcept = default;
  AVPacketWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  AVPacketWrapper(const ByteVector &data, std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  ~AVPacketWrapper() = default;

  // Todo: Needs to be tested
  [[nodiscard]] std::optional<AVPacketWrapper> clone() const;

  void setTimestamps(const int64_t dts, const int64_t pts);

  [[nodiscard]] libffmpeg::internal::AVPacket *getPacket() const { return this->packet.get(); }

  struct Flags
  {
    bool keyframe{};
    bool corrupt{};
    bool discard{};
  };

  [[nodiscard]] int                    getStreamIndex() const;
  [[nodiscard]] std::optional<int64_t> getPTS() const;
  [[nodiscard]] int64_t                getDTS() const;
  [[nodiscard]] int64_t                getDuration() const;
  [[nodiscard]] Flags                  getFlags() const;
  [[nodiscard]] int                    getDataSize() const;
  [[nodiscard]] ByteVector             getData() const;

  explicit operator bool() const { return this->packet != nullptr; };

private:
  void allocateNewPacket();

  class AVPacketDeleter
  {
  public:
    AVPacketDeleter() = default;
    AVPacketDeleter(const std::shared_ptr<IFFmpegLibraries> &ffmpegLibraries)
        : ffmpegLibraries(ffmpegLibraries) {};
    void operator()(libffmpeg::internal::AVPacket *packet) const noexcept;

  private:
    std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
  };

  std::unique_ptr<libffmpeg::internal::AVPacket, AVPacketDeleter> packet{nullptr,
                                                                         AVPacketDeleter()};
  std::shared_ptr<IFFmpegLibraries>                               ffmpegLibraries{};
};

} // namespace libffmpeg::avcodec
