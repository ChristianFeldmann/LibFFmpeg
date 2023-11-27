/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "InternalTypes.h"

namespace ffmpeg::internal
{

ColorSpace toColorspace(const AVColorSpace colorspace)
{
  switch (colorspace)
  {
  case AVCOL_SPC_RGB:
    return ColorSpace::RGB;
  case AVCOL_SPC_BT709:
    return ColorSpace::BT709;
  case AVCOL_SPC_UNSPECIFIED:
    return ColorSpace::UNSPECIFIED;
  case AVCOL_SPC_RESERVED:
    return ColorSpace::UNSPECIFIED;
  case AVCOL_SPC_FCC:
    return ColorSpace::FCC;
  case AVCOL_SPC_BT470BG:
    return ColorSpace::BT470BG;
  case AVCOL_SPC_SMPTE170M:
    return ColorSpace::SMPTE170M;
  case AVCOL_SPC_SMPTE240M:
    return ColorSpace::SMPTE240M;
  case AVCOL_SPC_YCOCG:
    return ColorSpace::YCOCG;
  case AVCOL_SPC_BT2020_NCL:
    return ColorSpace::BT2020_NCL;
  case AVCOL_SPC_BT2020_CL:
    return ColorSpace::BT2020_CL;
  case AVCOL_SPC_SMPTE2085:
    return ColorSpace::SMPTE2085;
  case AVCOL_SPC_CHROMA_DERIVED_NCL:
    return ColorSpace::CHROMA_DERIVED_NCL;
  case AVCOL_SPC_CHROMA_DERIVED_CL:
    return ColorSpace::CHROMA_DERIVED_CL;
  case AVCOL_SPC_ICTCP:
    return ColorSpace::ICTCP;

  default:
    return ColorSpace::UNSPECIFIED;
  }
}

MediaType toMediaType(const AVMediaType mediaType)
{
  switch (mediaType)
  {
  case AVMEDIA_TYPE_UNKNOWN:
    return MediaType::Unknown;
  case AVMEDIA_TYPE_VIDEO:
    return MediaType::Video;
  case AVMEDIA_TYPE_AUDIO:
    return MediaType::Audio;
  case AVMEDIA_TYPE_DATA:
    return MediaType::Data;
  case AVMEDIA_TYPE_SUBTITLE:
    return MediaType::Subtitle;
  case AVMEDIA_TYPE_ATTACHMENT:
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
    return AVMEDIA_TYPE_UNKNOWN;
  case MediaType::Video:
    return AVMEDIA_TYPE_VIDEO;
  case MediaType::Audio:
    return AVMEDIA_TYPE_AUDIO;
  case MediaType::Data:
    return AVMEDIA_TYPE_DATA;
  case MediaType::Subtitle:
    return AVMEDIA_TYPE_SUBTITLE;
  case MediaType::Attachment:
    return AVMEDIA_TYPE_ATTACHMENT;
  default:
    return AVMEDIA_TYPE_UNKNOWN;
  }
}

PictureType toPictureType(const AVPictureType pictureType)
{
  switch (pictureType)
  {
  case AV_PICTURE_TYPE_NONE:
    return PictureType::Undefined;
  case AV_PICTURE_TYPE_I:
    return PictureType::I;
  case AV_PICTURE_TYPE_P:
    return PictureType::P;
  case AV_PICTURE_TYPE_B:
    return PictureType::B;
  case AV_PICTURE_TYPE_S:
    return PictureType::S;
  case AV_PICTURE_TYPE_SI:
    return PictureType::SI;
  case AV_PICTURE_TYPE_SP:
    return PictureType::SP;
  case AV_PICTURE_TYPE_BI:
    return PictureType::BI;
  default:
    return PictureType::Undefined;
  }
}

} // namespace ffmpeg::internal
