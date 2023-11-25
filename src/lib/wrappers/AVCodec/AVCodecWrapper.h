/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorWrapper.h>

#include <memory>
#include <vector>

namespace ffmpeg::avcodec
{

class AVCodecWrapper
{
public:
  AVCodecWrapper() = default;
  AVCodecWrapper(AVCodec *codec, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  explicit operator bool() const { return this->codec != nullptr; }
  AVCodec *getAVCodec() const { return this->codec; }

  std::string                                    getName() const;
  std::string                                    getLongName() const;
  AVMediaType                                    getMediaType() const;
  AVCodecID                                      getCodecID() const;
  int                                            getCapabilities() const;
  std::vector<AVRational>                        getSupportedFramerates() const;
  std::vector<avutil::AVPixFmtDescriptorWrapper> getPixelFormats() const;
  std::vector<int>                               getSupportedSamplerates() const;
  std::vector<AVSampleFormat>                    getSampleFormats() const;
  std::vector<uint64_t>                          getChannelLayouts() const;
  uint8_t                                        getMaxLowres() const;

private:
  AVCodec *                                 codec{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avcodec
