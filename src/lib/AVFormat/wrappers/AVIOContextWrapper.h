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

class AVIOContextWrapper
{
public:
  AVIOContextWrapper()          = default;
  virtual ~AVIOContextWrapper() = 0;

  internal::AVIOContext *getAVIOContext() const;

  explicit operator bool() const { return !!this->ioContext; };

protected:
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

class AVIOInputContext : public AVIOContextWrapper
{
public:
  AVIOInputContext(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  virtual ~AVIOInputContext() = default;

  virtual std::optional<int> readData(uint8_t *buf, int buf_size) = 0;
  virtual std::optional<int> getFileSize() const                  = 0;
  virtual bool               seek(int64_t offset)                 = 0;
};

} // namespace libffmpeg::avformat
