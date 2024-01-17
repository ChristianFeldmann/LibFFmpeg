/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVCodecContextWrapper.h>
#include <AVCodec/wrappers/AVCodecDescriptorConversion.h>
#include <AVFormat/wrappers/AVCodecParametersWrapper.h>
#include <AVUtil/ColorSpace.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace ffmpeg::avformat
{

class AVStreamWrapper
{
public:
  AVStreamWrapper() = delete;
  AVStreamWrapper(ffmpeg::internal::AVStream       *stream,
                  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  explicit operator bool() const { return this->stream != nullptr; };

  int                           getIndex() const;
  avutil::MediaType             getCodecType() const;
  ffmpeg::internal::AVCodecID   getCodecID() const;
  Rational                      getAverageFrameRate() const;
  Rational                      getTimeBase() const;
  Size                          getFrameSize() const;
  avutil::ColorSpace            getColorspace() const;
  avutil::PixelFormatDescriptor getPixelFormat() const;
  ByteVector                    getExtradata() const;

  std::optional<avcodec::CodecDescriptor>       getCodecDescriptor() const;
  std::optional<AVCodecParametersWrapper>       getCodecParameters() const;
  std::optional<avcodec::AVCodecContextWrapper> getCodecContext() const;

private:
  ffmpeg::internal::AVStream       *stream{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avformat
