/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>

#include <vector>

namespace LibFFmpeg
{

class AVCodecWrapper
{
public:
  AVCodecWrapper() = default;
  AVCodecWrapper(AVCodec *codec, const LibraryVersions &libraryVersions);

  explicit operator bool() const { return this->codec != nullptr; }
  AVCodec *getAVCodec() { return this->codec; }

  std::string                 getName() const;
  std::string                 getLongName() const;
  AVMediaType                 getMediaType() const;
  AVCodecID                   getCodecID() const;
  int                         getCapabilities() const;
  std::vector<AVRational>     getSupportedFramerates() const;
  std::vector<AVPixelFormat>  getPixelFormats() const;
  std::vector<int>            getSupportedSamplerates() const;
  std::vector<AVSampleFormat> getSampleFormats() const;
  std::vector<uint64_t>       getChannelLayouts() const;
  uint8_t                     getMaxLowres() const;

private:
  AVCodec        *codec{nullptr};
  LibraryVersions libraryVersions;
};

} // namespace LibFFmpeg
