/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/ColorTypes.h>
#include <common/Enums.h>
#include <libHandling/IFFmpegLibraries.h>
#include <wrappers/AVFormat/AVCodecParametersWrapper.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorConversion.h>

namespace ffmpeg::avcodec
{

class AVCodecContextWrapper
{
public:
  AVCodecContextWrapper() = default;
  AVCodecContextWrapper(ffmpeg::internal::AVCodecContext *codecContext,
                        std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  static std::optional<AVCodecContextWrapper>
  openContextForDecoding(const avformat::AVCodecParametersWrapper &codecParameters,
                         std::shared_ptr<IFFmpegLibraries>         ffmpegLibraries);

  explicit operator bool() const { return this->codecContext != nullptr; };

  ffmpeg::internal::AVCodecContext *getCodecContext() const { return this->codecContext; }

  MediaType                     getCodecType() const;
  ffmpeg::internal::AVCodecID   getCodecID() const;
  avutil::PixelFormatDescriptor getPixelFormat() const;
  Size                          getSize() const;
  ColorSpace                    getColorspace() const;
  Rational                      getTimeBase() const;
  ByteVector                    getExtradata() const;

private:
  ffmpeg::internal::AVCodecContext *codecContext{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avcodec
