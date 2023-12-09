/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecDescriptorWrapper.h"

#include <wrappers/Functions.h>

#include "CastCodecClasses.h"

namespace ffmpeg::avcodec
{

namespace
{

using AVCodecDescriptor = ffmpeg::internal::AVCodecDescriptor;

constexpr auto AV_CODEC_PROP_INTRA_ONLY = (1 << 0);
constexpr auto AV_CODEC_PROP_LOSSY      = (1 << 1);
constexpr auto AV_CODEC_PROP_LOSSLESS   = (1 << 2);
constexpr auto AV_CODEC_PROP_REORDER    = (1 << 3);
constexpr auto AV_CODEC_PROP_BITMAP_SUB = (1 << 16);
constexpr auto AV_CODEC_PROP_TEXT_SUB   = (1 << 17);

constexpr auto FF_PROFILE_UNKNOWN = -99;

struct AVCodecDescriptor_56
{
  ffmpeg::internal::AVCodecID   id;
  ffmpeg::internal::AVMediaType type;
  const char                   *name;
  const char                   *long_name;
  int                           props;
  const char *const            *mime_types;
  const struct AVProfile       *profiles;
};

typedef AVCodecDescriptor_56 AVCodecDescriptor_57;
typedef AVCodecDescriptor_56 AVCodecDescriptor_58;
typedef AVCodecDescriptor_56 AVCodecDescriptor_59;
typedef AVCodecDescriptor_56 AVCodecDescriptor_60;

struct AVProfile
{
  int         profile;
  const char *name;
} AVProfile;

} // namespace

bool operator==(const AVCodecDescriptorProperties &lhs, const AVCodecDescriptorProperties &rhs)
{
  return (lhs.intraOnly == rhs.intraOnly && lhs.lossy == rhs.lossy &&
          lhs.lossless == rhs.lossless && lhs.reorder == rhs.reorder &&
          lhs.bitmapSub == rhs.bitmapSub && lhs.textSub == rhs.textSub);
}

AVCodecDescriptorWrapper::AVCodecDescriptorWrapper(const AVCodecDescriptor *codecDescriptor)
{
  const auto p = reinterpret_cast<const AVCodecDescriptor_56 *>(codecDescriptor);
  if (p == nullptr)
    return;

  this->mediaType = ffmpeg::internal::toMediaType(p->type);
  this->codecName = std::string(p->name);
  this->longName  = std::string(p->long_name);

  this->properties.intraOnly = (p->props & AV_CODEC_PROP_INTRA_ONLY);
  this->properties.lossy     = (p->props & AV_CODEC_PROP_LOSSY);
  this->properties.lossless  = (p->props & AV_CODEC_PROP_LOSSLESS);
  this->properties.reorder   = (p->props & AV_CODEC_PROP_REORDER);
  this->properties.bitmapSub = (p->props & AV_CODEC_PROP_BITMAP_SUB);
  this->properties.textSub   = (p->props & AV_CODEC_PROP_TEXT_SUB);

  if (p->mime_types != nullptr)
  {
    int i = 0;
    while (p->mime_types[i] != nullptr)
      this->mimeTypes.push_back(std::string(p->mime_types[i++]));
  }

  if (p->profiles != nullptr)
  {
    int i = 0;
    while (p->profiles[i].profile != FF_PROFILE_UNKNOWN)
      profiles.push_back(std::string(p->profiles[i++].name));
  }
}

bool AVCodecDescriptorWrapper::operator==(const AVCodecDescriptorWrapper &rhs) const
{
  return this->mediaType == rhs.mediaType && this->codecName == rhs.codecName &&
         this->longName == rhs.longName && this->properties == rhs.properties &&
         this->mimeTypes == rhs.mimeTypes && this->profiles == rhs.profiles;
}

} // namespace ffmpeg::avcodec
