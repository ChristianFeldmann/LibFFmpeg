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
    const internal::AVPixelFormat                    avPixelFormat,
    const std::shared_ptr<FFmpegLibrariesInterface> &librariesInterface)
{
  auto descriptor = librariesInterface->avutil.av_pix_fmt_desc_get(avPixelFormat);
  if (descriptor == nullptr)
  {
    this->name = "None";
    return;
  }

  const auto version = librariesInterface->getLibrariesVersion().avutil.major;

  if (version == 54)
  {
    auto p                   = reinterpret_cast<const AVPixFmtDescriptor_54 *>(descriptor);
    this->name               = std::string(p->name);
    this->numberOfComponents = static_cast<int>(p->nb_components);
    this->shiftLumaToChroma  = {static_cast<int>(p->log2_chroma_w),
                                static_cast<int>(p->log2_chroma_h)};
    this->flags              = parseFlagsFromValue(p->flags);

    for (int i = 0; i < this->numberOfComponents; ++i)
    {
      ComponentDescriptor descriptor;
      descriptor.plane  = p->comp[i].plane;
      descriptor.step   = p->comp[i].step_minus1 + 1;
      descriptor.offset = p->comp[i].offset_plus1 - 1;
      descriptor.shift  = p->comp[i].shift;
      descriptor.depth  = p->comp[i].depth_minus1 + 1;
      this->componentDescriptors.push_back(descriptor);
    }
  }
  else if (version == 55 || version == 56)
  {
    auto p                   = reinterpret_cast<const AVPixFmtDescriptor_55 *>(descriptor);
    this->name               = std::string(p->name);
    this->numberOfComponents = static_cast<int>(p->nb_components);
    this->shiftLumaToChroma  = {static_cast<int>(p->log2_chroma_w),
                                static_cast<int>(p->log2_chroma_h)};
    this->flags              = parseFlagsFromValue(p->flags);

    for (int i = 0; i < this->numberOfComponents; ++i)
    {
      ComponentDescriptor descriptor;
      descriptor.plane  = p->comp[i].plane;
      descriptor.step   = p->comp[i].step;
      descriptor.offset = p->comp[i].offset;
      descriptor.shift  = p->comp[i].shift;
      descriptor.depth  = p->comp[i].depth;
      this->componentDescriptors.push_back(descriptor);
    }
  }
  else if (version == 57 || version == 58)
  {
    auto p                   = reinterpret_cast<const AVPixFmtDescriptor_57 *>(descriptor);
    this->name               = std::string(p->name);
    this->numberOfComponents = static_cast<int>(p->nb_components);
    this->shiftLumaToChroma  = {static_cast<int>(p->log2_chroma_w),
                                static_cast<int>(p->log2_chroma_h)};
    this->flags              = parseFlagsFromValue(p->flags);

    for (int i = 0; i < this->numberOfComponents; ++i)
    {
      ComponentDescriptor descriptor;
      descriptor.plane  = p->comp[i].plane;
      descriptor.step   = p->comp[i].step;
      descriptor.offset = p->comp[i].offset;
      descriptor.shift  = p->comp[i].shift;
      descriptor.depth  = p->comp[i].depth;
      this->componentDescriptors.push_back(descriptor);
    }
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace ffmpeg::avutil
