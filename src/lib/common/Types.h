/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace libffmpeg
{

struct Size
{
  int width{};
  int height{};
};

inline bool operator==(const Size &lhs, const Size &rhs)
{
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

struct Rational
{
  bool operator!=(const Rational &second) const
  {
    const auto a = int64_t(this->numerator) * int64_t(second.denominator);
    const auto b = int64_t(this->denominator) * int64_t(second.numerator);
    return a != b;
  }
  bool operator==(const Rational &second) const
  {
    const auto a = int64_t(this->numerator) * int64_t(second.denominator);
    const auto b = int64_t(this->denominator) * int64_t(second.numerator);
    return a == b;
  }

  int numerator;
  int denominator;
};

using ByteVector = std::vector<std::byte>;
using Path       = std::filesystem::path;

} // namespace libffmpeg
