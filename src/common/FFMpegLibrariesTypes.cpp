/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FFMpegLibrariesTypes.h"

#include <iomanip>
#include <iostream>

namespace LibFFmpeg
{

std::string timestampToString(int64_t timestamp, AVRational timebase)
{
  auto absolutSeconds = static_cast<double>(timestamp) * static_cast<double>(timebase.num) /
                        static_cast<double>(timebase.den);

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

Version Version::fromFFmpegVersion(const unsigned ffmpegVersion)
{
  Version v;
  v.major = AV_VERSION_MAJOR(ffmpegVersion);
  v.minor = AV_VERSION_MINOR(ffmpegVersion);
  v.micro = AV_VERSION_MICRO(ffmpegVersion);
  return v;
}

std::string to_string(const Version &version)
{
  std::ostringstream stream;
  stream << "v" << version.major;
  if (version.minor)
  {
    stream << "." << version.minor.value();
    if (version.micro)
      stream << "." << version.micro.value();
  }
  return stream.str();
}

std::ostream &operator<<(std::ostream &stream, const Version &version)
{
  stream << to_string(version);
  return stream;
}

} // namespace LibFFmpeg
