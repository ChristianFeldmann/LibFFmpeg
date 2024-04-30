/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/MediaType.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace libffmpeg::avcodec
{

struct CodecDescriptorProperties
{
  bool intraOnly{};
  bool lossy{};
  bool lossless{};
  bool reorder{};
  bool bitmapSub{};
  bool textSub{};
};

struct CodecDescriptor
{
  avutil::MediaType         mediaType{avutil::MediaType::Unknown};
  std::string               codecName{"Unknown"};
  std::string               longName{};
  CodecDescriptorProperties properties{};
  std::vector<std::string>  mimeTypes{};
  std::vector<std::string>  profiles{};
};

bool operator==(const CodecDescriptorProperties &lhs, const CodecDescriptorProperties &rhs);

CodecDescriptor convertAVCodecDescriptor(const internal::AVCodecDescriptor *avCodecDescriptor,
                                         const Version &                    avCodecVersion);

} // namespace libffmpeg::avcodec
