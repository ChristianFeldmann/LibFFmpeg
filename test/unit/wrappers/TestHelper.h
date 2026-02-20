/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Version.h>

#include <array>
#include <string>

#include <gtest/gtest.h>

namespace libffmpeg
{

static std::string getNameWithFFmpegVersion(const testing::TestParamInfo<LibraryVersions> &info)
{
  const auto version = info.param.ffmpegVersion;
  auto       name    = "FFmpeg_" + ffmpegVersionMapper.getName(version);
  std::ranges::replace(name, '.', '_');
  return name;
}

template <std::size_t SIZE>
ByteVector dataArrayToByteVector(const std::array<uint8_t, SIZE> dataArray)
{
  ByteVector dataVector;
  for (const auto value : dataArray)
    dataVector.push_back(std::byte(value));
  return dataVector;
}

template <typename Iter> std::vector<Rational> toRationalVector(Iter begin, Iter end)
{
  std::vector<Rational> rationals;
  while (begin != end)
  {
    rationals.push_back(Rational({begin->num, begin->den}));
    ++begin;
  }
  return rationals;
}

} // namespace libffmpeg
