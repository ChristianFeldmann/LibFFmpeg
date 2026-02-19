/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFrameSideDataWrapper.h"

#include "AVFrameSideDataWrapperInternal.h"
#include "CastUtilClasses.h"

#include <stdexcept>

namespace libffmpeg::avutil
{

namespace
{

using libffmpeg::internal::AVFrameSideData;
using libffmpeg::internal::AVFrameSideDataType;

} // namespace

AVFrameSideDataWrapper::AVFrameSideDataWrapper(AVFrameSideData                  *sideData,
                                               std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : sideData(sideData), ffmpegLibraries(ffmpegLibraries)
{
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
}

std::vector<MotionVector> AVFrameSideDataWrapper::getMotionVectors() const
{
  if (this->sideData == nullptr)
    return {};

  AVFrameSideDataType type{};
  CAST_AVUTIL_GET_MEMBER(AVFrameSideData, this->sideData, type, type);

  if (type != libffmpeg::internal::AV_FRAME_DATA_MOTION_VECTORS)
    return {};

  uint8_t *data{};
  CAST_AVUTIL_GET_MEMBER(AVFrameSideData, this->sideData, data, data);

  int size{};
  CAST_AVUTIL_GET_MEMBER(AVFrameSideData, this->sideData, size, size);

  return parseMotionData(this->ffmpegLibraries->getLibrariesVersion(), data, size);
}

} // namespace libffmpeg::avutil
