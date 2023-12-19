/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace ffmpeg::avutil
{

struct PixelFormatDescriptor
{
  /**
   * Amount to shift the luma width/height right to find the chroma width/height.
   * For YV12 this is 1 for example.
   * chroma_width = -((-luma_width) >> log2_chroma_w)
   * The note above is needed to ensure rounding up.
   * This value only refers to the chroma components.
   */
  struct Shift
  {
    int  widthShift{};
    int  heightShift{};
    bool operator==(const Shift &other) const;
  };

  /**
   * Parameters that describe how pixels are packed.
   * If the format has 2 or 4 components, then alpha is last.
   * If the format has 1 or 2 components, then luma is 0.
   * If the format has 3 or 4 components,
   * if the RGB flag is set then 0 is red, 1 is green and 2 is blue;
   * otherwise 0 is luma, 1 is chroma-U and 2 is chroma-V.
   */
  struct ComponentDescriptor
  {
    int  plane{};  ///< which of the 4 planes contains the component
    int  step{};   ///< Number of elements between 2 horizontally consecutive pixels
    int  offset{}; ///< Number of elements before the component of the first pixel
    int  shift{};  ///< number of least significant bits that must be shifted away to get the value
    int  depth{};  ///< number of bits in the component
    bool operator==(const ComponentDescriptor &other) const;
  };

  struct Flags
  {
    bool bigEndian{};
    bool pallette{};
    bool bitwisePacked{};
    bool hwAccelerated{};
    bool planar{};
    bool rgb{};
    bool pseudoPallette{};
    bool hasAlphaPlane{};
    bool bayerPattern{};
    bool floatValues{};
    bool operator==(const Flags &other) const;
  };

  std::string                      name{"Unknown"};
  int                              numberOfComponents{};
  Shift                            shiftLumaToChroma{};
  Flags                            flags{};
  std::vector<ComponentDescriptor> componentDescriptors;

  bool operator==(const PixelFormatDescriptor &other) const;
};

PixelFormatDescriptor
convertAVPixFmtDescriptor(const internal::AVPixelFormat            avPixelFormat,
                          const std::shared_ptr<IFFmpegLibraries> &ffmpegLibraries);

} // namespace ffmpeg::avutil
