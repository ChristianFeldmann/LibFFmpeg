/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/ColorTypes.h>

namespace ffmpeg::internal
{

/* These enums are only used internally. So far, these enums did not change between the FFmpeg
 * versions. If they ever do change, we will have to add a wrapper around them.
 */

enum AVColorSpace
{
  AVCOL_SPC_RGB                = 0,
  AVCOL_SPC_BT709              = 1,
  AVCOL_SPC_UNSPECIFIED        = 2,
  AVCOL_SPC_RESERVED           = 3,
  AVCOL_SPC_FCC                = 4,
  AVCOL_SPC_BT470BG            = 5,
  AVCOL_SPC_SMPTE170M          = 6,
  AVCOL_SPC_SMPTE240M          = 7,
  AVCOL_SPC_YCOCG              = 8,
  AVCOL_SPC_BT2020_NCL         = 9,
  AVCOL_SPC_BT2020_CL          = 10,
  AVCOL_SPC_SMPTE2085          = 11,
  AVCOL_SPC_CHROMA_DERIVED_NCL = 12,
  AVCOL_SPC_CHROMA_DERIVED_CL  = 13,
  AVCOL_SPC_ICTCP              = 14
};

ColorSpace toColorspace(const AVColorSpace colorspace);

/* The pixel format is very different per FFmpeg version. It actually depends on how FFMpeg
   was compiled. So having a wrapper around this does not make sense. We will only handle
   corresponding AVPixelFormatDescriptors which we can get from FFMpeg.
*/
enum AVPixelFormat
{
  AV_PIX_FMT_NONE = -1
};

} // namespace ffmpeg::internal
