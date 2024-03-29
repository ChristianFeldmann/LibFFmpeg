/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/MediaType.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>
#include <vector>

namespace ffmpeg::avcodec
{

class AVCodecWrapper
{
public:
  AVCodecWrapper() = delete;
  AVCodecWrapper(ffmpeg::internal::AVCodec        *codec,
                 std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  explicit                   operator bool() const { return this->codec != nullptr; }
  ffmpeg::internal::AVCodec *getAVCodec() const { return this->codec; }

  std::string                                   getName() const;
  std::string                                   getLongName() const;
  avutil::MediaType                             getMediaType() const;
  ffmpeg::internal::AVCodecID                   getCodecID() const;
  int                                           getCapabilities() const;
  std::vector<Rational>                         getSupportedFramerates() const;
  std::vector<avutil::PixelFormatDescriptor>    getPixelFormats() const;
  std::vector<int>                              getSupportedSamplerates() const;
  std::vector<ffmpeg::internal::AVSampleFormat> getSampleFormats() const;
  std::vector<uint64_t>                         getChannelLayouts() const;
  uint8_t                                       getMaxLowres() const;

private:
  ffmpeg::internal::AVCodec        *codec{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avcodec
