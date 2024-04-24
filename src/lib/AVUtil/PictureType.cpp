/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "PictureType.h"

namespace libffmpeg::avutil
{

using libffmpeg::internal::AVPictureType;

PictureType toPictureType(const AVPictureType pictureType)
{
  switch (pictureType)
  {
  case AVPictureType::AV_PICTURE_TYPE_NONE:
    return PictureType::Undefined;
  case AVPictureType::AV_PICTURE_TYPE_I:
    return PictureType::I;
  case AVPictureType::AV_PICTURE_TYPE_P:
    return PictureType::P;
  case AVPictureType::AV_PICTURE_TYPE_B:
    return PictureType::B;
  case AVPictureType::AV_PICTURE_TYPE_S:
    return PictureType::S;
  case AVPictureType::AV_PICTURE_TYPE_SI:
    return PictureType::SI;
  case AVPictureType::AV_PICTURE_TYPE_SP:
    return PictureType::SP;
  case AVPictureType::AV_PICTURE_TYPE_BI:
    return PictureType::BI;
  default:
    return PictureType::Undefined;
  }
}

} // namespace libffmpeg::avutil
