/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <AVFormat/wrappers/AVCodecParametersWrapper.h>
#include <AVUtil/ColorSpace.h>
#include <AVUtil/MediaType.h>
#include <AVUtil/wrappers/AVFrameWrapper.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <common/Error.h>
#include <libHandling/IFFmpegLibraries.h>

namespace ffmpeg::avcodec
{

class AVCodecContextWrapper
{
public:
  AVCodecContextWrapper() = delete;
  AVCodecContextWrapper(ffmpeg::internal::AVCodecContext *codecContext,
                        std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  AVCodecContextWrapper(const AVCodecContextWrapper &) = delete;
  AVCodecContextWrapper &operator=(AVCodecContextWrapper &&);
  AVCodecContextWrapper &operator=(const AVCodecContextWrapper &) = delete;
  AVCodecContextWrapper(AVCodecContextWrapper &&wrapper);
  ~AVCodecContextWrapper();

  static std::optional<AVCodecContextWrapper>
  openContextForDecoding(const avformat::AVCodecParametersWrapper &codecParameters,
                         std::shared_ptr<IFFmpegLibraries>         ffmpegLibraries);

  ReturnCode sendPacket(const avcodec::AVPacketWrapper &packet);
  ReturnCode sendFlushPacket();

  struct RevieveFrameResult
  {
    std::optional<avutil::AVFrameWrapper> frame{};
    ReturnCode                            returnCode{};
  };
  RevieveFrameResult revieveFrame();

  avutil::MediaType             getCodecType() const;
  ffmpeg::internal::AVCodecID   getCodecID() const;
  avutil::PixelFormatDescriptor getPixelFormat() const;
  Size                          getSize() const;
  avutil::ColorSpace            getColorspace() const;
  Rational                      getTimeBase() const;
  ByteVector                    getExtradata() const;

private:
  /* It depends on how the wrapper is created who has ownership of this.
   * If the function openContextForDecoding is used, then this class has ownership. If the
   * constructor with a raw pointer is called, then the ownership is not in this class. That is used
   * for older FFmpeg versions where the AVStream has ownership of the codec. In newer versions,
   * this is not the case anymore.
   */
  ffmpeg::internal::AVCodecContext *codecContext{};
  bool                              codecContextOwnership{};

  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avcodec
