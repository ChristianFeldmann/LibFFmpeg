/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
