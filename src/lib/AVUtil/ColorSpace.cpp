/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "ColorSpace.h"

namespace ffmpeg::avutil
{

using ffmpeg::internal::AVColorSpace;

ColorSpace toColorspace(const AVColorSpace colorspace)
{
  switch (colorspace)
  {
  case AVColorSpace::AVCOL_SPC_RGB:
    return ColorSpace::RGB;
  case AVColorSpace::AVCOL_SPC_BT709:
    return ColorSpace::BT709;
  case AVColorSpace::AVCOL_SPC_UNSPECIFIED:
    return ColorSpace::UNSPECIFIED;
  case AVColorSpace::AVCOL_SPC_RESERVED:
    return ColorSpace::UNSPECIFIED;
  case AVColorSpace::AVCOL_SPC_FCC:
    return ColorSpace::FCC;
  case AVColorSpace::AVCOL_SPC_BT470BG:
    return ColorSpace::BT470BG;
  case AVColorSpace::AVCOL_SPC_SMPTE170M:
    return ColorSpace::SMPTE170M;
  case AVColorSpace::AVCOL_SPC_SMPTE240M:
    return ColorSpace::SMPTE240M;
  case AVColorSpace::AVCOL_SPC_YCOCG:
    return ColorSpace::YCOCG;
  case AVColorSpace::AVCOL_SPC_BT2020_NCL:
    return ColorSpace::BT2020_NCL;
  case AVColorSpace::AVCOL_SPC_BT2020_CL:
    return ColorSpace::BT2020_CL;
  case AVColorSpace::AVCOL_SPC_SMPTE2085:
    return ColorSpace::SMPTE2085;
  case AVColorSpace::AVCOL_SPC_CHROMA_DERIVED_NCL:
    return ColorSpace::CHROMA_DERIVED_NCL;
  case AVColorSpace::AVCOL_SPC_CHROMA_DERIVED_CL:
    return ColorSpace::CHROMA_DERIVED_CL;
  case AVColorSpace::AVCOL_SPC_ICTCP:
    return ColorSpace::ICTCP;

  default:
    return ColorSpace::UNSPECIFIED;
  }
}

} // namespace ffmpeg::avutil
