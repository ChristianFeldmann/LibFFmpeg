/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/FFmpegLibrariesInterface.h>

#include <memory>

namespace ffmpeg::avcodec
{

struct AVCodecDescriptorProperties
{
  bool intraOnly{};
  bool lossy{};
  bool lossless{};
  bool reorder{};
  bool bitmapSub{};
  bool textSub{};
};

bool operator==(const AVCodecDescriptorProperties &lhs, const AVCodecDescriptorProperties &rhs);

class AVCodecDescriptorWrapper
{
public:
  AVCodecDescriptorWrapper(const AVCodecDescriptor *codecDescriptor);

  AVMediaType getMediaType() const { return this->mediaType; }
  std::string getCodecName() const { return this->codecName; }
  std::string getLongName() const { return this->longName; }

  AVCodecDescriptorProperties getProperties() const { return this->properties; }
  std::vector<std::string>    getMimeTypes() const { return this->mimeTypes; }
  std::vector<std::string>    getProfiles() const { return this->profiles; }

private:
  AVMediaType                 mediaType{AVMEDIA_TYPE_UNKNOWN};
  std::string                 codecName{};
  std::string                 longName{};
  AVCodecDescriptorProperties properties{};
  std::vector<std::string>    mimeTypes;
  std::vector<std::string>    profiles;
};

} // namespace ffmpeg::avcodec
