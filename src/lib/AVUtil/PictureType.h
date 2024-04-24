/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/EnumMapper.h>
#include <common/InternalTypes.h>

namespace libffmpeg::avutil
{

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

PictureType toPictureType(const libffmpeg::internal::AVPictureType pictureType);

} // namespace libffmpeg::avutil
