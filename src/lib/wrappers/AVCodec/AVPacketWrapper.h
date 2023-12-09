/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace ffmpeg::avcodec
{

// A wrapper around the different versions of the AVPacket versions.
// It also adds some functions like automatic deletion when it goes out of scope.
class AVPacketWrapper
{
public:
  AVPacketWrapper() = default;
  AVPacketWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  AVPacketWrapper(ffmpeg::internal::AVPacket       *packet,
                  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  AVPacketWrapper(const ByteVector &data, std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  void allocatePacket();

  void setTimestamps(const int64_t dts, const int64_t pts);

  ffmpeg::internal::AVPacket *getPacket() const { return this->packet; }

  struct Flags
  {
    bool keyframe{};
    bool corrupt{};
    bool discard{};
  };

  int        getStreamIndex() const;
  int64_t    getPTS() const;
  int64_t    getDTS() const;
  int64_t    getDuration() const;
  Flags      getFlags() const;
  int        getDataSize() const;
  ByteVector getData() const;

  explicit operator bool() const { return this->packet != nullptr; };

private:
  ffmpeg::internal::AVPacket       *packet{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avcodec
