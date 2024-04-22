/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVMotionVectorConversion.h"

#include "AVMotionVectorConversionInternal.h"

#include <stdexcept>

namespace ffmpeg::avutil
{

using internal::avutil::AVMotionVector_54;
using internal::avutil::AVMotionVector_55_56_57_58_59;

std::vector<MotionVector>
parseMotionData(const LibraryVersions &libraryVersions, const uint8_t *data, const size_t dataSize)
{
  std::vector<MotionVector> motionVectors;
  if (libraryVersions.avutil.major == 54)
  {
    const auto nrMotionVectors = dataSize / sizeof(AVMotionVector_54);
    for (size_t index = 0; index < nrMotionVectors; ++index)
    {
      const auto byteOffset = sizeof(AVMotionVector_54) * index;
      const auto p          = reinterpret_cast<const AVMotionVector_54 *>(data + byteOffset);

      MotionVector motionVector;
      motionVector.source = p->source;
      motionVector.w      = p->w;
      motionVector.h      = p->h;
      motionVector.src_x  = p->src_x;
      motionVector.src_y  = p->src_y;
      motionVector.dst_x  = p->dst_x;
      motionVector.dst_y  = p->dst_y;
      motionVector.flags  = p->flags;
      motionVectors.push_back(motionVector);
    }
    return motionVectors;
  }
  else if (libraryVersions.avutil.major == 55 || //
           libraryVersions.avutil.major == 56 || //
           libraryVersions.avutil.major == 57 || //
           libraryVersions.avutil.major == 58 || //
           libraryVersions.avutil.major == 59)
  {
    const auto nrMotionVectors = dataSize / sizeof(AVMotionVector_55_56_57_58_59);
    for (size_t index = 0; index < nrMotionVectors; ++index)
    {
      const auto byteOffset = sizeof(AVMotionVector_55_56_57_58_59) * index;
      const auto p = reinterpret_cast<const AVMotionVector_55_56_57_58_59 *>(data + byteOffset);

      MotionVector motionVector;
      motionVector.source       = p->source;
      motionVector.w            = p->w;
      motionVector.h            = p->h;
      motionVector.src_x        = p->src_x;
      motionVector.src_y        = p->src_y;
      motionVector.dst_x        = p->dst_x;
      motionVector.dst_y        = p->dst_y;
      motionVector.flags        = p->flags;
      motionVector.motion_x     = p->motion_x;
      motionVector.motion_y     = p->motion_y;
      motionVector.motion_scale = p->motion_scale;
      motionVectors.push_back(motionVector);
    }
    return motionVectors;
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace ffmpeg::avutil
