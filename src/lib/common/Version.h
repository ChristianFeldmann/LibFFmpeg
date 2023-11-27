/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <optional>
#include <ostream>
#include <string>

namespace ffmpeg
{

struct Version
{
  Version() = default;
  Version(const int major) { this->major = major; }

  bool operator!=(const Version &other) const;

  std::string   toString() const;
  std::ostream &operator<<(std::ostream &stream) const;

  static Version fromFFmpegVersion(const unsigned ffmpegVersion);

  int                major{};
  std::optional<int> minor{};
  std::optional<int> micro{};
};

struct LibraryVersions
{
  Version avformat{};
  Version avcodec{};
  Version avutil{};
  Version swresample{};
};

} // namespace ffmpeg
