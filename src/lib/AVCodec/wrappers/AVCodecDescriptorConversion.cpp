/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecDescriptorConversion.h"

#include <common/Functions.h>

#include "AVCodecDescriptorConversionInternal.h"
#include "CastCodecClasses.h"

namespace libffmpeg::avcodec
{

namespace
{

using libffmpeg::internal::AVCodecDescriptor;
using libffmpeg::internal::avcodec::AVCodecDescriptor_56;
using libffmpeg::internal::avcodec::AVCodecDescriptor_57;
using libffmpeg::internal::avcodec::AVProfile_57;

constexpr auto AV_CODEC_PROP_INTRA_ONLY = (1 << 0);
constexpr auto AV_CODEC_PROP_LOSSY      = (1 << 1);
constexpr auto AV_CODEC_PROP_LOSSLESS   = (1 << 2);
constexpr auto AV_CODEC_PROP_REORDER    = (1 << 3);
constexpr auto AV_CODEC_PROP_BITMAP_SUB = (1 << 16);
constexpr auto AV_CODEC_PROP_TEXT_SUB   = (1 << 17);

constexpr auto FF_PROFILE_UNKNOWN = -99;

} // namespace

bool operator==(const CodecDescriptorProperties &lhs, const CodecDescriptorProperties &rhs)
{
  return (lhs.intraOnly == rhs.intraOnly && lhs.lossy == rhs.lossy &&
          lhs.lossless == rhs.lossless && lhs.reorder == rhs.reorder &&
          lhs.bitmapSub == rhs.bitmapSub && lhs.textSub == rhs.textSub);
}

CodecDescriptorProperties parseProperties(int props)
{
  CodecDescriptorProperties properties;

  properties.intraOnly = (props & AV_CODEC_PROP_INTRA_ONLY);
  properties.lossy     = (props & AV_CODEC_PROP_LOSSY);
  properties.lossless  = (props & AV_CODEC_PROP_LOSSLESS);
  properties.reorder   = (props & AV_CODEC_PROP_REORDER);
  properties.bitmapSub = (props & AV_CODEC_PROP_BITMAP_SUB);
  properties.textSub   = (props & AV_CODEC_PROP_TEXT_SUB);

  return properties;
}

std::vector<std::string> parseMimeTypes(const char *const *mime_types)
{
  if (mime_types == nullptr)
    return {};

  std::vector<std::string> mimeTypes;

  int i = 0;
  while (mime_types[i] != nullptr)
    mimeTypes.push_back(std::string(mime_types[i++]));

  return mimeTypes;
}

std::vector<std::string> parseProfiles(const struct AVProfile_57 *profiles)
{
  if (profiles == nullptr)
    return {};

  std::vector<std::string> parsedProfiles;

  int i = 0;
  while (profiles[i].profile != FF_PROFILE_UNKNOWN)
    parsedProfiles.push_back(std::string(profiles[i++].name));

  return parsedProfiles;
}

CodecDescriptor convertAVCodecDescriptor(const internal::AVCodecDescriptor *avCodecDescriptor,
                                         const Version &                    avCodecVersion)
{
  if (avCodecDescriptor == nullptr)
    throw std::runtime_error("Invalid avCodecDescriptor given");

  CodecDescriptor descriptor;

  if (avCodecVersion.major == 56)
  {
    const auto p          = reinterpret_cast<const AVCodecDescriptor_56 *>(avCodecDescriptor);
    descriptor.mediaType  = libffmpeg::avutil::toMediaType(p->type);
    descriptor.codecName  = std::string(p->name);
    descriptor.longName   = std::string(p->long_name);
    descriptor.properties = parseProperties(p->props);
    descriptor.mimeTypes  = parseMimeTypes(p->mime_types);
  }
  else
  {
    const auto p          = reinterpret_cast<const AVCodecDescriptor_57 *>(avCodecDescriptor);
    descriptor.mediaType  = libffmpeg::avutil::toMediaType(p->type);
    descriptor.codecName  = std::string(p->name);
    descriptor.longName   = std::string(p->long_name);
    descriptor.properties = parseProperties(p->props);
    descriptor.mimeTypes  = parseMimeTypes(p->mime_types);
    descriptor.profiles   = parseProfiles(p->profiles);
  }

  return descriptor;
}

} // namespace libffmpeg::avcodec
