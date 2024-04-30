/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "MediaType.h"

namespace libffmpeg::avutil
{

using libffmpeg::internal::AVMediaType;

MediaType toMediaType(const AVMediaType mediaType)
{
  switch (mediaType)
  {
  case AVMediaType::AVMEDIA_TYPE_UNKNOWN:
    return MediaType::Unknown;
  case AVMediaType::AVMEDIA_TYPE_VIDEO:
    return MediaType::Video;
  case AVMediaType::AVMEDIA_TYPE_AUDIO:
    return MediaType::Audio;
  case AVMediaType::AVMEDIA_TYPE_DATA:
    return MediaType::Data;
  case AVMediaType::AVMEDIA_TYPE_SUBTITLE:
    return MediaType::Subtitle;
  case AVMediaType::AVMEDIA_TYPE_ATTACHMENT:
    return MediaType::Attachment;
  default:
    return MediaType::Unknown;
  }
}

AVMediaType toAVMediaType(const MediaType mediaType)
{
  switch (mediaType)
  {
  case MediaType::Unknown:
    return AVMediaType::AVMEDIA_TYPE_UNKNOWN;
  case MediaType::Video:
    return AVMediaType::AVMEDIA_TYPE_VIDEO;
  case MediaType::Audio:
    return AVMediaType::AVMEDIA_TYPE_AUDIO;
  case MediaType::Data:
    return AVMediaType::AVMEDIA_TYPE_DATA;
  case MediaType::Subtitle:
    return AVMediaType::AVMEDIA_TYPE_SUBTITLE;
  case MediaType::Attachment:
    return AVMediaType::AVMEDIA_TYPE_ATTACHMENT;
  default:
    return AVMediaType::AVMEDIA_TYPE_UNKNOWN;
  }
}

} // namespace libffmpeg::avutil
