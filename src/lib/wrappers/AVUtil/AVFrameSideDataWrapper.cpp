/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFrameSideDataWrapper.h"

#include "CastUtilClasses.h"

#include <stdexcept>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AVBufferRef;
using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVFrameSideData;
using ffmpeg::internal::AVFrameSideDataType;

// Part of AVUtil
struct AVFrameSideData_54
{
  AVFrameSideDataType type;
  uint8_t            *data;
  int                 size;
  AVDictionary       *metadata;
  AVBufferRef        *buf;
};

typedef AVFrameSideData_54 AVFrameSideData_55;
typedef AVFrameSideData_54 AVFrameSideData_56;

struct AVFrameSideData_57
{
  AVFrameSideDataType type;
  uint8_t            *data;
  size_t              size;
  AVDictionary       *metadata;
  AVBufferRef        *buf;
};

typedef AVFrameSideData_57 AVFrameSideData_58;

} // namespace

AVFrameSideDataWrapper::AVFrameSideDataWrapper(
    AVFrameSideData *sideData, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : sideData(sideData), librariesInterface(librariesInterface)
{
}

std::vector<MotionVector> AVFrameSideDataWrapper::getMotionVectors() const
{
  if (this->sideData == nullptr)
    return {};

  AVFrameSideDataType type;
  CAST_AVUTIL_GET_MEMBER(AVFrameSideData, this->sideData, type, type);

  if (type != ffmpeg::internal::AV_FRAME_DATA_MOTION_VECTORS)
    return {};

  uint8_t *data;
  CAST_AVUTIL_GET_MEMBER(AVFrameSideData, this->sideData, data, data);

  int size;
  CAST_AVUTIL_GET_MEMBER(AVFrameSideData, this->sideData, size, size);

  return parseMotionData(this->librariesInterface->getLibrariesVersion(), data, size);
}

} // namespace ffmpeg::avutil
