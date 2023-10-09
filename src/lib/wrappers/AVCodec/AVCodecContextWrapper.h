/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>

namespace ffmpeg::avcodec
{

class AVCodecContextWrapper
{
public:
  AVCodecContextWrapper() = default;
  AVCodecContextWrapper(AVCodecContext                           *c,
                        std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  explicit operator bool() const { return this->codecContext != nullptr; };

  AVCodecContext *getCodecContext() const { return this->codecContext; }

  AVMediaType   getCodecType() const;
  AVCodecID     getCodecID() const;
  AVPixelFormat getPixelFormat() const;
  Size          getSize() const;
  AVColorSpace  getColorspace() const;
  AVRational    getTimeBase() const;
  ByteVector    getExtradata() const;

private:
  AVCodecContext                           *codecContext{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avcodec
