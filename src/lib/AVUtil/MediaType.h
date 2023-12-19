/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/EnumMapper.h>
#include <common/InternalTypes.h>

namespace ffmpeg::avutil
{

enum class MediaType
{
  Unknown,
  Video,
  Audio,
  Data,
  Subtitle,
  Attachment
};

const EnumMapper<MediaType> mediaTypeMapper({{MediaType::Unknown, "Unknown"},
                                             {MediaType::Video, "Video"},
                                             {MediaType::Audio, "Audio"},
                                             {MediaType::Data, "Data"},
                                             {MediaType::Subtitle, "Subtitle"},
                                             {MediaType::Attachment, "Attachment"}});

MediaType                     toMediaType(const ffmpeg::internal::AVMediaType mediaType);
ffmpeg::internal::AVMediaType toAVMediaType(const MediaType mediaType);

} // namespace ffmpeg::avutil
