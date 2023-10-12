/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVPixFmtDescriptorWrapper.h"

#include "CastUtilClasses.h"

using namespace std::rel_ops;

namespace ffmpeg::avutil
{

namespace
{

struct AVComponentDescriptor_54
{
  uint16_t plane : 2;
  uint16_t step_minus1 : 3;
  uint16_t offset_plus1 : 3;
  uint16_t shift : 3;
  uint16_t depth_minus1 : 4;
};

struct AVPixFmtDescriptor_54
{
  const char              *name;
  uint8_t                  nb_components;
  uint8_t                  log2_chroma_w;
  uint8_t                  log2_chroma_h;
  uint8_t                  flags;
  AVComponentDescriptor_54 comp[4];
  const char              *alias;
};

struct AVComponentDescriptor_55
{
  int plane;
  int step;
  int offset;
  int shift;
  int depth;

  // deprectaed
  int step_minus1;
  int depth_minus1;
  int offset_plus1;
};

typedef AVComponentDescriptor_55 AVComponentDescriptor_56;

struct AVPixFmtDescriptor_55
{
  const char              *name;
  uint8_t                  nb_components;
  uint8_t                  log2_chroma_w;
  uint8_t                  log2_chroma_h;
  uint64_t                 flags;
  AVComponentDescriptor_55 comp[4];
  const char              *alias;
};

typedef AVPixFmtDescriptor_55 AVPixFmtDescriptor_56;

struct AVComponentDescriptor_57
{
  int plane;
  int step;
  int offset;
  int shift;
  int depth;
};

struct AVPixFmtDescriptor_57
{
  const char              *name;
  uint8_t                  nb_components;
  uint8_t                  log2_chroma_w;
  uint8_t                  log2_chroma_h;
  uint64_t                 flags;
  AVComponentDescriptor_57 comp[4];
  const char              *alias;
};

typedef AVPixFmtDescriptor_57 AVPixFmtDescriptor_58;

AVPixFmtDescriptorWrapper::Flags parseFlagsFromValue(const uint64_t flagsValue)
{
  AVPixFmtDescriptorWrapper::Flags flags;
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

AVPixFmtDescriptorWrapper::AVPixFmtDescriptorWrapper(
    AVPixFmtDescriptor                       *pixelFormatDescriptor,
    std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : pixelFormatDescriptor(pixelFormatDescriptor), librariesInterface(librariesInterface)
{
}

std::string AVPixFmtDescriptorWrapper::getName() const
{
  const char *name;
  CAST_AVUTIL_GET_MEMBER(AVPixFmtDescriptor, this->pixelFormatDescriptor, name, name);
  return std::string(name);
}

int AVPixFmtDescriptorWrapper::getNumberOfComponents() const
{
  int numberComponents;
  CAST_AVUTIL_GET_MEMBER(
      AVPixFmtDescriptor, this->pixelFormatDescriptor, numberComponents, nb_components);
  return numberComponents;
}

AVPixFmtDescriptorWrapper::Shift AVPixFmtDescriptorWrapper::getShiftLumaToChroma() const
{
  int width;
  CAST_AVUTIL_GET_MEMBER(AVPixFmtDescriptor, this->pixelFormatDescriptor, width, log2_chroma_w);

  int height;
  CAST_AVUTIL_GET_MEMBER(AVPixFmtDescriptor, this->pixelFormatDescriptor, height, log2_chroma_h);

  return {width, height};
}

AVPixFmtDescriptorWrapper::Flags AVPixFmtDescriptorWrapper::getFlags() const
{
  uint64_t flagsValue;
  CAST_AVUTIL_GET_MEMBER(AVPixFmtDescriptor, this->pixelFormatDescriptor, flagsValue, flags);

  return parseFlagsFromValue(flagsValue);
}

std::vector<AVPixFmtDescriptorWrapper::ComponentDescriptor>
AVPixFmtDescriptorWrapper::getComponentDescriptors() const
{
  std::vector<ComponentDescriptor> descriptors;

  const auto version = this->librariesInterface->getLibrariesVersion().avutil.major;

  for (int component = 0; component < this->getNumberOfComponents(); ++component)
  {
    ComponentDescriptor descriptor;

    if (version == 54)
    {
      const auto p      = reinterpret_cast<AVPixFmtDescriptor_54 *>(this->pixelFormatDescriptor);
      descriptor.plane  = p->comp[component].plane;
      descriptor.step   = p->comp[component].step_minus1 + 1;
      descriptor.offset = p->comp[component].offset_plus1 - 1;
      descriptor.shift  = p->comp[component].shift;
      descriptor.depth  = p->comp[component].depth_minus1 + 1;
    }
    if (version == 55 || version == 56)
    {
      const auto p      = reinterpret_cast<AVPixFmtDescriptor_55 *>(this->pixelFormatDescriptor);
      descriptor.plane  = p->comp[component].plane;
      descriptor.step   = p->comp[component].step;
      descriptor.offset = p->comp[component].offset;
      descriptor.shift  = p->comp[component].shift;
      descriptor.depth  = p->comp[component].depth;
    }
    if (version == 57 || version == 58)
    {
      const auto p      = reinterpret_cast<AVPixFmtDescriptor_57 *>(this->pixelFormatDescriptor);
      descriptor.plane  = p->comp[component].plane;
      descriptor.step   = p->comp[component].step;
      descriptor.offset = p->comp[component].offset;
      descriptor.shift  = p->comp[component].shift;
      descriptor.depth  = p->comp[component].depth;
    }
    else
      throw std::runtime_error("Invalid library version");

    descriptors.push_back(descriptor);
  }

  return descriptors;
}

} // namespace ffmpeg::avutil
