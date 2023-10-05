/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <common/MotionVector.h>

namespace LibFFmpeg
{

class AVFrameSideDataWrapper
{
public:
  AVFrameSideDataWrapper() = default;
  AVFrameSideDataWrapper(AVFrameSideData *sideData, const LibraryVersions &libraryVersions);

  std::vector<MotionVector> getMotionVectors() const;

  explicit operator bool() const { return sideData != nullptr; }

private:
  AVFrameSideData *sideData{};
  LibraryVersions  libraryVersions{};
};

} // namespace LibFFmpeg
