/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/FFmpegLibrariesInterface.h>

#include <memory>

namespace ffmpeg::avutil
{

class AVPixFmtDescriptorWrapper
{
public:
  AVPixFmtDescriptorWrapper() = default;
  AVPixFmtDescriptorWrapper(AVPixFmtDescriptor                       *pixelFormatDescriptor,
                            std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  /**
   * Amount to shift the luma width/height right to find the chroma width/height.
   * For YV12 this is 1 for example.
   * chroma_width = -((-luma_width) >> log2_chroma_w)
   * The note above is needed to ensure rounding up.
   * This value only refers to the chroma components.
   */
  struct Shift
  {
    int widthShift{};
    int heightShift{};
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
    int plane{};  ///< which of the 4 planes contains the component
    int step{};   ///< Number of elements between 2 horizontally consecutive pixels
    int offset{}; ///< Number of elements before the component of the first pixel
    int shift{};  ///< number of least significant bits that must be shifted away to get the value
    int depth{};  ///< number of bits in the component
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
  };

  std::string                      getName() const;
  int                              getNumberOfComponents() const;
  Shift                            getShiftLumaToChroma() const;
  Flags                            getFlags() const;
  std::vector<ComponentDescriptor> getComponentDescriptors() const;

private:
  AVPixFmtDescriptor                       *pixelFormatDescriptor{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avutil
