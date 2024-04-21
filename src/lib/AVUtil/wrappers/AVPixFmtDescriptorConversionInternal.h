/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace ffmpeg::internal::avutil
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
typedef AVPixFmtDescriptor_57 AVPixFmtDescriptor_59;

} // namespace ffmpeg::internal::avutil
