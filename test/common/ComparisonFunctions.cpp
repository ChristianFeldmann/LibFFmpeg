/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "ComparisonFunctions.h"

#include <cstddef>

namespace ffmpeg
{

bool areEqual(const std::vector<std::string> &lhs, const std::vector<std::string> &rhs)
{
  if (lhs.size() != rhs.size())
    return false;

  for (size_t i = 0; i < lhs.size(); ++i)
  {
    if (lhs.at(i) != rhs.at(i))
      return false;
  }
  return true;
}

std::int64_t calculateFrameDataHash(const avutil::AVFrameWrapper &frame)
{
  const auto pixelFormatDescriptor = frame.getPixelFormatDescriptor();
  const auto frameSize             = frame.getSize();

  std::int64_t hash = 0;

  for (int component = 0; component < pixelFormatDescriptor.numberOfComponents; ++component)
  {
    const auto data     = frame.getData(component);
    const auto dataSize = data.size();
    for (const auto value : data)
      hash ^= std::to_integer<std::int64_t>(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
  }

  return hash;
}

} // namespace ffmpeg
