/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/MediaType.h>
#include <libHandling/IFFmpegLibraries.h>

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

/* The interface is a bit different on this class because it did not change since it was first
 * created. So we do not need the version of the ffmpeg libraries to use this. If the struct is
 * changed in the future, then we will have to add the libffmpeg shared pointer to the constructor.
 */

class AVCodecDescriptorWrapper
{
public:
  AVCodecDescriptorWrapper() = delete;
  AVCodecDescriptorWrapper(const ffmpeg::internal::AVCodecDescriptor *codecDescriptor);

  avutil::MediaType getMediaType() const { return this->mediaType; }
  std::string       getCodecName() const { return this->codecName; }
  std::string       getLongName() const { return this->longName; }

  AVCodecDescriptorProperties getProperties() const { return this->properties; }
  std::vector<std::string>    getMimeTypes() const { return this->mimeTypes; }
  std::vector<std::string>    getProfiles() const { return this->profiles; }

  bool operator==(const AVCodecDescriptorWrapper &rhs) const;

private:
  avutil::MediaType           mediaType{avutil::MediaType::Unknown};
  std::string                 codecName{"Unknown"};
  std::string                 longName{};
  AVCodecDescriptorProperties properties{};
  std::vector<std::string>    mimeTypes{};
  std::vector<std::string>    profiles{};
};

} // namespace ffmpeg::avcodec
