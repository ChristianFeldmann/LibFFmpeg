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

enum class PictureType
{
  Undefined,
  I,
  P,
  B,
  S,
  SI,
  SP,
  BI
};

const EnumMapper<PictureType>
    PictureTypeMapper({{PictureType::Undefined, "Undefined"},
                       {PictureType::I, "I-Frame", "Intra frame"},
                       {PictureType::P, "P-Frame", "Predictively coded frame"},
                       {PictureType::B, "B-Frame", "Bi-directionally predicttively coded frame"},
                       {PictureType::S, "S-Frame", "S(GMC)-VOP MPEG4 frame"},
                       {PictureType::SI, "SI-Frame", "Switching intra frame"},
                       {PictureType::SP, "SP-Frame", "Switching predictively coded frame"},
                       {PictureType::SI, "SI-Frame", "Switching bi-predictively coded frame"}});

} // namespace ffmpeg
