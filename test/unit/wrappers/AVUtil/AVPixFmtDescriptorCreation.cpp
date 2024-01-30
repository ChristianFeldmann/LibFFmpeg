/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVPixFmtDescriptorCreation.h"
#include <AVUtil/wrappers/AVPixFmtDescriptorConversionInternal.h>

namespace ffmpeg::avutil
{

using ffmpeg::internal::avutil::AVComponentDescriptor_54;
using ffmpeg::internal::avutil::AVComponentDescriptor_55;
using ffmpeg::internal::avutil::AVComponentDescriptor_56;
using ffmpeg::internal::avutil::AVComponentDescriptor_57;
using ffmpeg::internal::avutil::AVPixFmtDescriptor_54;
using ffmpeg::internal::avutil::AVPixFmtDescriptor_55;
using ffmpeg::internal::avutil::AVPixFmtDescriptor_56;
using ffmpeg::internal::avutil::AVPixFmtDescriptor_57;
using ffmpeg::internal::avutil::AVPixFmtDescriptor_58;

namespace
{

template <typename AVComponentDescriptorType>
void setComponentDescriptorValues(AVComponentDescriptorType &                      dstDescriptor,
                                  const PixelFormatDescriptor::ComponentDescriptor srcDescriptor)
{
  dstDescriptor.shift = srcDescriptor.shift;

  if constexpr (std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_54> ||
                std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_55>)
  {
    dstDescriptor.step_minus1  = srcDescriptor.step - 1;
    dstDescriptor.offset_plus1 = srcDescriptor.offset + 1;
    dstDescriptor.depth_minus1 = srcDescriptor.depth - 1;
  }
  if constexpr (std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_55> ||
                std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_57>)
  {
    dstDescriptor.step   = srcDescriptor.step;
    dstDescriptor.offset = srcDescriptor.offset;
    dstDescriptor.depth  = srcDescriptor.depth;
  }
}

} // namespace

template <typename AVPixFmtDescriptorType>
AVPixFmtDescriptorType createRawFormatDescriptor(const PixelFormatDescriptor &pixelFormat)
{
  AVPixFmtDescriptorType descriptor;

  descriptor.name          = pixelFormat.name.c_str();
  descriptor.nb_components = pixelFormat.numberOfComponents;
  descriptor.log2_chroma_w = pixelFormat.shiftLumaToChroma.widthShift;
  descriptor.log2_chroma_h = pixelFormat.shiftLumaToChroma.heightShift;
  descriptor.flags         = 0b10001; // Planar and bigEndia flag

  for (int i = 0; i < pixelFormat.componentDescriptors.size(); ++i)
  {
    descriptor.comp[i].plane = i;
    setComponentDescriptorValues(descriptor.comp[i], pixelFormat.componentDescriptors.at(i));
  }

  return descriptor;
}

template AVPixFmtDescriptor_54
createRawFormatDescriptor<AVPixFmtDescriptor_54>(const PixelFormatDescriptor &pixelFormat);
template AVPixFmtDescriptor_55
createRawFormatDescriptor<AVPixFmtDescriptor_55>(const PixelFormatDescriptor &pixelFormat);
template AVPixFmtDescriptor_57
createRawFormatDescriptor<AVPixFmtDescriptor_57>(const PixelFormatDescriptor &pixelFormat);

} // namespace ffmpeg::avutil
