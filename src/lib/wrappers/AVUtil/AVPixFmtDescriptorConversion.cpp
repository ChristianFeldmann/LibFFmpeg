/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVPixFmtDescriptorConversion.h"

#include "AVPixFmtDescriptorConversionInternal.h"
#include "CastUtilClasses.h"

using namespace std::rel_ops;

namespace ffmpeg::avutil
{

namespace
{

PixelFormatDescriptor::Flags parseFlagsFromValue(const uint64_t flagsValue)
{
  PixelFormatDescriptor::Flags flags;
  flags.bigEndian      = flagsValue & (1 << 0);
  flags.pallette       = flagsValue & (1 << 1);
  flags.bitwisePacked  = flagsValue & (1 << 2);
  flags.hwAccelerated  = flagsValue & (1 << 3);
  flags.planar         = flagsValue & (1 << 4);
  flags.rgb            = flagsValue & (1 << 5);
  flags.pseudoPallette = flagsValue & (1 << 6);
  flags.hasAlphaPlane  = flagsValue & (1 << 7);
  flags.bayerPattern   = flagsValue & (1 << 8);
  flags.floatValues    = flagsValue & (1 << 9);
  return flags;
}

} // namespace

PixelFormatDescriptor
convertAVPixFmtDescriptor(const internal::AVPixelFormat            avPixelFormat,
                          const std::shared_ptr<IFFmpegLibraries> &ffmpegLibraries)
{
  PixelFormatDescriptor format;

  auto descriptor = ffmpegLibraries->avutil.av_pix_fmt_desc_get(avPixelFormat);
  if (descriptor == nullptr)
  {
    format.name = "None";
    return format;
  }

  const auto version = ffmpegLibraries->getLibrariesVersion().avutil.major;

  if (version == 54)
  {
    auto p      = reinterpret_cast<const internal::avutil::AVPixFmtDescriptor_54 *>(descriptor);
    format.name = std::string(p->name);
    format.numberOfComponents = static_cast<int>(p->nb_components);
    format.shiftLumaToChroma  = {static_cast<int>(p->log2_chroma_w),
                                 static_cast<int>(p->log2_chroma_h)};
    format.flags              = parseFlagsFromValue(p->flags);

    for (int i = 0; i < format.numberOfComponents; ++i)
    {
      PixelFormatDescriptor::ComponentDescriptor descriptor;
      descriptor.plane  = p->comp[i].plane;
      descriptor.step   = p->comp[i].step_minus1 + 1;
      descriptor.offset = p->comp[i].offset_plus1 - 1;
      descriptor.shift  = p->comp[i].shift;
      descriptor.depth  = p->comp[i].depth_minus1 + 1;
      format.componentDescriptors.push_back(descriptor);
    }
  }
  else if (version == 55 || version == 56)
  {
    auto p      = reinterpret_cast<const internal::avutil::AVPixFmtDescriptor_55 *>(descriptor);
    format.name = std::string(p->name);
    format.numberOfComponents = static_cast<int>(p->nb_components);
    format.shiftLumaToChroma  = {static_cast<int>(p->log2_chroma_w),
                                 static_cast<int>(p->log2_chroma_h)};
    format.flags              = parseFlagsFromValue(p->flags);

    for (int i = 0; i < format.numberOfComponents; ++i)
    {
      PixelFormatDescriptor::ComponentDescriptor descriptor;
      descriptor.plane  = p->comp[i].plane;
      descriptor.step   = p->comp[i].step;
      descriptor.offset = p->comp[i].offset;
      descriptor.shift  = p->comp[i].shift;
      descriptor.depth  = p->comp[i].depth;
      format.componentDescriptors.push_back(descriptor);
    }
  }
  else if (version == 57 || version == 58)
  {
    auto p      = reinterpret_cast<const internal::avutil::AVPixFmtDescriptor_57 *>(descriptor);
    format.name = std::string(p->name);
    format.numberOfComponents = static_cast<int>(p->nb_components);
    format.shiftLumaToChroma  = {static_cast<int>(p->log2_chroma_w),
                                 static_cast<int>(p->log2_chroma_h)};
    format.flags              = parseFlagsFromValue(p->flags);

    for (int i = 0; i < format.numberOfComponents; ++i)
    {
      PixelFormatDescriptor::ComponentDescriptor descriptor;
      descriptor.plane  = p->comp[i].plane;
      descriptor.step   = p->comp[i].step;
      descriptor.offset = p->comp[i].offset;
      descriptor.shift  = p->comp[i].shift;
      descriptor.depth  = p->comp[i].depth;
      format.componentDescriptors.push_back(descriptor);
    }
  }
  else
    throw std::runtime_error("Invalid library version");

  return format;
}

} // namespace ffmpeg::avutil
