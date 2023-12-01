/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFrameSideDataWrapper.h"

#include "AVFrameSideDataWrapperInternal.h"
#include "CastUtilClasses.h"

#include <stdexcept>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AVFrameSideData;
using ffmpeg::internal::AVFrameSideDataType;

} // namespace

AVFrameSideDataWrapper::AVFrameSideDataWrapper(AVFrameSideData                  *sideData,
                                               std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : sideData(sideData), ffmpegLibraries(ffmpegLibraries)
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

  return parseMotionData(this->ffmpegLibraries->getLibrariesVersion(), data, size);
}

} // namespace ffmpeg::avutil
