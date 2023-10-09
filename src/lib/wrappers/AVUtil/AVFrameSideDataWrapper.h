/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <common/MotionVector.h>
#include <libHandling/FFmpegLibrariesInterface.h>

#include <memory>

namespace ffmpeg::avutil
{

class AVFrameSideDataWrapper
{
public:
  AVFrameSideDataWrapper() = default;
  AVFrameSideDataWrapper(AVFrameSideData                          *sideData,
                         std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  std::vector<MotionVector> getMotionVectors() const;

  explicit operator bool() const { return sideData != nullptr; }

private:
  AVFrameSideData                          *sideData{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avutil
