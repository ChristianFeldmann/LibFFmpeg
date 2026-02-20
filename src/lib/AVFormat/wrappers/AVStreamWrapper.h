/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVCodecContextWrapper.h>
#include <AVCodec/wrappers/AVCodecDescriptorConversion.h>
#include <AVCodec/wrappers/AVCodecParametersWrapper.h>
#include <AVUtil/ColorSpace.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace libffmpeg::avformat
{

class AVStreamWrapper
{
public:
  AVStreamWrapper() = delete;
  AVStreamWrapper(libffmpeg::internal::AVStream    *stream,
                  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  explicit operator bool() const { return this->stream != nullptr; };

  [[nodiscard]] int                            getIndex() const;
  [[nodiscard]] avutil::MediaType              getCodecType() const;
  [[nodiscard]] libffmpeg::internal::AVCodecID getCodecID() const;
  [[nodiscard]] Rational                       getAverageFrameRate() const;
  [[nodiscard]] Rational                       getTimeBase() const;
  [[nodiscard]] Size                           getFrameSize() const;
  [[nodiscard]] avutil::ColorSpace             getColorspace() const;
  [[nodiscard]] avutil::PixelFormatDescriptor  getPixelFormat() const;
  [[nodiscard]] ByteVector                     getExtradata() const;

  [[nodiscard]] std::optional<avcodec::CodecDescriptor>          getCodecDescriptor() const;
  [[nodiscard]] std::optional<avcodec::AVCodecParametersWrapper> getCodecParameters() const;
  [[nodiscard]] std::optional<avcodec::AVCodecContextWrapper>    getCodecContext() const;

private:
  libffmpeg::internal::AVStream    *stream{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace libffmpeg::avformat
