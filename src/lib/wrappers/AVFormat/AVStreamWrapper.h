/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/ColorTypes.h>
#include <libHandling/IFFmpegLibraries.h>
#include <wrappers/AVCodec/AVCodecContextWrapper.h>
#include <wrappers/AVCodec/AVCodecDescriptorWrapper.h>
#include <wrappers/AVFormat/AVCodecParametersWrapper.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorConversion.h>

#include <memory>

namespace ffmpeg::avformat
{

class AVStreamWrapper
{
public:
  AVStreamWrapper() = default;
  AVStreamWrapper(ffmpeg::internal::AVStream       *stream,
                  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  explicit operator bool() const { return this->stream != nullptr; };

  int                               getIndex() const;
  MediaType                         getCodecType() const;
  ffmpeg::internal::AVCodecID       getCodecID() const;
  avcodec::AVCodecDescriptorWrapper getCodecDescriptor() const;
  Rational                          getAverageFrameRate() const;
  Rational                          getTimeBase() const;
  Size                              getFrameSize() const;
  ColorSpace                        getColorspace() const;
  avutil::PixelFormatDescriptor     getPixelFormat() const;
  ByteVector                        getExtradata() const;

  std::optional<AVCodecParametersWrapper> getCodecParameters() const;

private:
  ffmpeg::internal::AVStream       *stream{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};

  avcodec::AVCodecContextWrapper getCodecContext() const;
};

} // namespace ffmpeg::avformat
