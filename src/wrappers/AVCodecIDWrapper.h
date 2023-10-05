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

namespace LibFFmpeg
{

class AVCodecIDWrapper
{
public:
  AVCodecIDWrapper() {}
  AVCodecIDWrapper(AVCodecID codecID, std::string codecName)
      : codecID(codecID), codecName(codecName)
  {
  }

  std::string getCodecName() const { return this->codecName; }
  AVCodecID   getCodecID() const { return this->codecID; }

  void setCodecID(AVCodecID id) { this->codecID = id; }

  void setTypeHEVC() { this->codecName = "hevc"; }
  void setTypeAVC() { this->codecName = "h264"; }

  bool isHEVC() const { return this->codecName == "hevc"; }
  bool isAVC() const { return this->codecName == "h264"; }
  bool isMpeg2() const { return this->codecName == "mpeg2video"; }
  bool isAV1() const { return this->codecName == "av1"; }

  bool isNone() const
  {
    return this->codecName.empty() || this->codecName == "unknown_codec" ||
           this->codecName == "none";
  }

  bool operator==(const AVCodecIDWrapper &a) const { return codecID == a.codecID; }

private:
  AVCodecID   codecID{AV_CODEC_ID_NONE};
  std::string codecName;
};

} // namespace LibFFmpeg
