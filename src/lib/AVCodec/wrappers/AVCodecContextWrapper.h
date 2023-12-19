/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVFormat/wrappers/AVCodecParametersWrapper.h>
#include <AVUtil/ColorSpace.h>
#include <AVUtil/MediaType.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <libHandling/IFFmpegLibraries.h>

namespace ffmpeg::avcodec
{

class AVCodecContextWrapper
{
public:
  AVCodecContextWrapper() = delete;
  AVCodecContextWrapper(ffmpeg::internal::AVCodecContext *codecContext,
                        std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  static std::optional<AVCodecContextWrapper>
  openContextForDecoding(const avformat::AVCodecParametersWrapper &codecParameters,
                         std::shared_ptr<IFFmpegLibraries>         ffmpegLibraries);

  // Todo: Remove this and give this codec context pushPacket / pullFrame functions
  ffmpeg::internal::AVCodecContext *getCodecContext() const { return this->codecContext; }

  avutil::MediaType             getCodecType() const;
  ffmpeg::internal::AVCodecID   getCodecID() const;
  avutil::PixelFormatDescriptor getPixelFormat() const;
  Size                          getSize() const;
  avutil::ColorSpace            getColorspace() const;
  Rational                      getTimeBase() const;
  ByteVector                    getExtradata() const;

private:
  ffmpeg::internal::AVCodecContext *codecContext{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avcodec
