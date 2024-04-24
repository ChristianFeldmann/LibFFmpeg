/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Version.h>

namespace libffmpeg
{

#define RUN_TEST_FOR_VERSION(version, testFunctionName)                                            \
  {                                                                                                \
    if (version.ffmpegVersion == libffmpeg::FFmpegVersion::FFmpeg_2x)                                 \
      testFunctionName<libffmpeg::FFmpegVersion::FFmpeg_2x>();                                        \
    else if (version.ffmpegVersion == libffmpeg::FFmpegVersion::FFmpeg_3x)                            \
      testFunctionName<libffmpeg::FFmpegVersion::FFmpeg_3x>();                                        \
    else if (version.ffmpegVersion == libffmpeg::FFmpegVersion::FFmpeg_4x)                            \
      testFunctionName<libffmpeg::FFmpegVersion::FFmpeg_4x>();                                        \
    else if (version.ffmpegVersion == libffmpeg::FFmpegVersion::FFmpeg_5x)                            \
      testFunctionName<libffmpeg::FFmpegVersion::FFmpeg_5x>();                                        \
    else if (version.ffmpegVersion == libffmpeg::FFmpegVersion::FFmpeg_6x)                            \
      testFunctionName<libffmpeg::FFmpegVersion::FFmpeg_6x>();                                        \
  }

template <typename T> struct TypeWrapper
{
  using type = T;
};

constexpr LibraryVersions getLibraryVerions(const FFmpegVersion ffmpegVersion)
{
  // Starting with C++ 20 we could also use std::find_if in the SupportedFFmpegVersions list
  auto it = SupportedFFmpegVersions.begin();
  while (it != SupportedFFmpegVersions.end())
  {
    if (it->ffmpegVersion == ffmpegVersion)
      return *it;
    ++it;
  }

  return {};
}

} // namespace libffmpeg