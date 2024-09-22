/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>
#include <libHandling/IFFmpegLibraries.h>

#include <optional>

namespace libffmpeg::avformat
{

class AVIOInputContext
{
public:
  AVIOInputContext(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  virtual ~AVIOInputContext() = default;

  internal::AVIOContext *getAVIOContext() const;

  virtual std::optional<int> read_packet(uint8_t *buf, int buf_size) = 0;

  explicit operator bool() const { return !!this->ioContext; };

private:
  class AVIOContextDeleter
  {
  public:
    AVIOContextDeleter() = default;
    AVIOContextDeleter(const std::shared_ptr<IFFmpegLibraries> &ffmpegLibraries)
        : ffmpegLibraries(ffmpegLibraries) {};
    void operator()(internal::AVIOContext *ioContext) const noexcept;

  private:
    std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
  };

  std::unique_ptr<internal::AVIOContext, AVIOContextDeleter> ioContext{nullptr,
                                                                       AVIOContextDeleter()};
};

} // namespace libffmpeg::avformat
