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

  AVCodecID   getCodecID();
  std::string getName();
  std::string getLongName();

private:
  void update();

  std::string                 name{};
  std::string                 long_name{};
  AVMediaType                 type;
  AVCodecID                   id{AV_CODEC_ID_NONE};
  int                         capabilities{0};
  std::vector<AVRational>     supported_framerates;
  std::vector<AVPixelFormat>  pix_fmts;
  std::vector<int>            supported_samplerates;
  std::vector<AVSampleFormat> sample_fmts;
  std::vector<uint64_t>       channel_layouts;
  uint8_t                     max_lowres{0};

  AVCodec *       codec{nullptr};
  LibraryVersions libraryVersions;
};

} // namespace LibFFmpeg
