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

enum class ColorSpace
{
  UNSPECIFIED,
  RGB,        ///<
  BT709,      ///< also ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
  FCC,        ///<
  BT470BG,    ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL &SECAM / IEC
              ///< 61966-2-4 xvYCC601
  SMPTE170M,  ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC
  SMPTE240M,  ///< functionally identical to above
  YCOCG,      ///<
  BT2020_NCL, ///< ITU-R BT2020 non-constant luminance system
  BT2020_CL,  ///< ITU-R BT2020 constant luminance system
  SMPTE2085,  ///< SMPTE 2085, Y'D'zD'x
  CHROMA_DERIVED_NCL, ///< Chromaticity-derived non-constant luminance system
  CHROMA_DERIVED_CL,  ///<
  ICTCP,              ///< ITU-R BT.2100-0, ICtCp
};

const EnumMapper<ColorSpace> colorSpaceMapper(
    {{ColorSpace::UNSPECIFIED, "Unspecified", "Unspecified"},
     {ColorSpace::RGB, "RGB", "GBR, also IEC 61966-2-1 (sRGB)"},
     {ColorSpace::BT709, "BT.709", "ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B"},
     {ColorSpace::FCC, "FCC", "FCC Title 47 Code of Federal Regulations 73.682 (a)(20)"},
     {ColorSpace::BT470BG,
      "BT.470BG",
      "also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL &SECAM / IEC 61966-2-4 "
      "xvYCC601"},
     {ColorSpace::SMPTE170M,
      "SMPTE170M",
      "SMPTE170M / ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC"},
     {ColorSpace::SMPTE240M,
      "SMPTE240M",
      "SMPTE240M / ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC"},
     {ColorSpace::YCOCG, "YCOCG", "Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16"},
     {ColorSpace::BT2020_NCL, "BT2020_NCL", "ITU-R BT2020 non-constant luminance system"},
     {ColorSpace::BT2020_CL, "BT2020_CL", "ITU-R BT2020 constant luminance system"},
     {ColorSpace::SMPTE2085, "SMPTE2085", "SMPTE 2085, Y'D'zD'x"},
     {ColorSpace::CHROMA_DERIVED_NCL,
      "CHROMA_DERIVED_NCL",
      "Chromaticity-derived non-constant luminance system"},
     {ColorSpace::CHROMA_DERIVED_CL,
      "CHROMA_DERIVED_CL",
      "Chromaticity-derived constant luminance system"},
     {ColorSpace::ICTCP, "Unspecified", "ITU-R BT.2100-0, ICtCp"}});

ColorSpace toColorspace(const ffmpeg::internal::AVColorSpace colorspace);

} // namespace ffmpeg::avutil
