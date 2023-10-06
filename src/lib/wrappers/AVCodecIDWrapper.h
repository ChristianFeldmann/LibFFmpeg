/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
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
