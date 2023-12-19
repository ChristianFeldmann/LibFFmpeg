/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Formatting.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace ffmpeg
{

std::string toString(int64_t timestamp, Rational timebase)
{
  auto absolutSeconds = static_cast<double>(timestamp) * static_cast<double>(timebase.numerator) /
                        static_cast<double>(timebase.denominator);

  const auto hours = static_cast<int>(absolutSeconds / 60 / 60);
  absolutSeconds -= hours * 60 * 60;

  const auto minutes = static_cast<int>(absolutSeconds / 60);
  absolutSeconds -= minutes * 60;

  const auto seconds = static_cast<int>(absolutSeconds);
  absolutSeconds -= seconds;

  auto milliseconds = static_cast<int>(absolutSeconds * 1000);

  std::ostringstream stream;
  stream << std::setfill('0') << std::setw(2) << hours << ":";
  stream << std::setfill('0') << std::setw(2) << minutes << ":";
  stream << std::setfill('0') << std::setw(2) << seconds << ".";
  stream << std::setfill('0') << std::setw(3) << milliseconds << ":";

  return stream.str();
}

} // namespace ffmpeg
