/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVCodec/AVCodecContextWrapper.h>
#include <wrappers/AVCodec/AVCodecIDWrapper.h>
#include <wrappers/AVFormat/AVCodecParametersWrapper.h>

#include <memory>

namespace ffmpeg::avformat
{

class AVStreamWrapper
{
public:
  AVStreamWrapper() = default;
  AVStreamWrapper(AVStream *stream, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  explicit operator bool() const { return this->stream != nullptr; };

  AVMediaType   getCodecType() const;
  std::string   getCodecTypeName() const;
  AVCodecID     getCodecID() const;
  AVRational    getAverageFrameRate() const;
  AVRational    getTimeBase() const;
  Size          getFrameSize() const;
  AVColorSpace  getColorspace() const;
  AVPixelFormat getPixelFormat() const;
  ByteVector    getExtradata() const;
  int           getIndex() const;

  avcodec::AVCodecContextWrapper getCodecContext() const;
  AVCodecParametersWrapper       getCodecParameters() const;

private:
  AVStream                                 *stream{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avformat
