/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "EnumMapper.h"

namespace ffmpeg
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

} // namespace ffmpeg
