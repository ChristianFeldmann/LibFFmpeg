/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <string>
#include <vector>

namespace ffmpeg
{

inline bool areEqual(const std::vector<std::string> &lhs, const std::vector<std::string> &rhs)
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

} // namespace ffmpeg
