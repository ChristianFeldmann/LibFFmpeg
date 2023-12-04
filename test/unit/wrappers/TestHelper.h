/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Version.h>

#include <string>

#include <gtest/gtest.h>

namespace ffmpeg
{

static std::string getNameWithFFmpegVersion(const testing::TestParamInfo<LibraryVersions> &info)
{
  const auto version = info.param.ffmpegVersion;
  auto       name    = "FFmpeg_" + ffmpegVersionMapper.getName(version);
  std::replace(name.begin(), name.end(), '.', '_');
  return name;
}

} // namespace ffmpeg
