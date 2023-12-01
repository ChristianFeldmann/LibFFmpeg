/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "AVMotionVectorConversion.h"

#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace ffmpeg::avutil
{

class AVFrameSideDataWrapper
{
public:
  AVFrameSideDataWrapper() = default;
  AVFrameSideDataWrapper(ffmpeg::internal::AVFrameSideData *sideData,
                         std::shared_ptr<IFFmpegLibraries>  ffmpegLibraries);

  std::vector<MotionVector> getMotionVectors() const;

  explicit operator bool() const { return sideData != nullptr; }

private:
  ffmpeg::internal::AVFrameSideData *sideData{};
  std::shared_ptr<IFFmpegLibraries>  ffmpegLibraries{};
};

} // namespace ffmpeg::avutil
