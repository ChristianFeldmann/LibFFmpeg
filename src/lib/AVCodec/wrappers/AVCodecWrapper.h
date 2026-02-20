/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/MediaType.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <libHandling/IFFmpegLibraries.h>

#include "../Channel.h"

#include <memory>
#include <vector>

namespace libffmpeg::avcodec
{

class AVCodecWrapper
{
public:
  AVCodecWrapper() = delete;
  AVCodecWrapper(libffmpeg::internal::AVCodec     *codec,
                 std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  explicit operator bool() const { return this->codec != nullptr; }
  [[nodiscard]] libffmpeg::internal::AVCodec *getAVCodec() const { return this->codec; }

  [[nodiscard]] std::string                                      getName() const;
  [[nodiscard]] std::string                                      getLongName() const;
  [[nodiscard]] avutil::MediaType                                getMediaType() const;
  [[nodiscard]] libffmpeg::internal::AVCodecID                   getCodecID() const;
  [[nodiscard]] int                                              getCapabilities() const;
  [[nodiscard]] std::vector<Rational>                            getSupportedFramerates() const;
  [[nodiscard]] std::vector<avutil::PixelFormatDescriptor>       getPixelFormats() const;
  [[nodiscard]] std::vector<int>                                 getSupportedSamplerates() const;
  [[nodiscard]] std::vector<libffmpeg::internal::AVSampleFormat> getSampleFormats() const;
  [[nodiscard]] std::vector<ChannelLayout>                       getSupportedChannelLayouts() const;
  [[nodiscard]] uint8_t                                          getMaxLowres() const;

private:
  libffmpeg::internal::AVCodec     *codec{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace libffmpeg::avcodec
