/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/EnumMapper.h>

#include <optional>
#include <ostream>
#include <string>

namespace ffmpeg
{

struct Version
{
  constexpr Version() = default;
  constexpr Version(const int major) : major(major) {}

  bool operator!=(const Version &other) const;

  std::string   toString() const;
  std::ostream &operator<<(std::ostream &stream) const;

  static Version fromFFmpegVersion(const unsigned ffmpegVersion);

  int                major{};
  std::optional<int> minor{};
  std::optional<int> micro{};
};

enum class FFmpegVersion
{
  FFmpeg_2x,
  FFmpeg_3x,
  FFmpeg_4x,
  FFmpeg_5x,
  FFmpeg_6x,
  FFmpeg_7x
};

const EnumMapper<FFmpegVersion>
    ffmpegVersionMapper({{FFmpegVersion::FFmpeg_2x, "2.x", "FFmpeg 2.x"},
                         {FFmpegVersion::FFmpeg_3x, "3.x", "FFmpeg 3.x"},
                         {FFmpegVersion::FFmpeg_4x, "4.x", "FFmpeg 4.x"},
                         {FFmpegVersion::FFmpeg_5x, "5.x", "FFmpeg 5.x"},
                         {FFmpegVersion::FFmpeg_6x, "6.x", "FFmpeg 6.x"},
                         {FFmpegVersion::FFmpeg_7x, "7.x", "FFmpeg 7.x"}});

struct LibraryVersions
{
  FFmpegVersion ffmpegVersion{};
  Version       avformat{};
  Version       avcodec{};
  Version       avutil{};
  Version       swresample{};
};

using VersionAVFormat   = Version;
using VersionAVCodec    = Version;
using VersionAVUtil     = Version;
using VersionSwresample = Version;

// These FFmpeg versions are supported. The numbers indicate the major versions.
// The versions are sorted from newest to oldest, so that we try to open the newest ones first.
constexpr auto SupportedFFmpegVersions = {LibraryVersions({FFmpegVersion::FFmpeg_7x,
                                                           VersionAVFormat(61),
                                                           VersionAVCodec(61),
                                                           VersionAVUtil(59),
                                                           VersionSwresample(5)}),
                                          LibraryVersions({FFmpegVersion::FFmpeg_6x,
                                                           VersionAVFormat(60),
                                                           VersionAVCodec(60),
                                                           VersionAVUtil(58),
                                                           VersionSwresample(4)}),
                                          LibraryVersions({FFmpegVersion::FFmpeg_5x,
                                                           VersionAVFormat(59),
                                                           VersionAVCodec(59),
                                                           VersionAVUtil(57),
                                                           VersionSwresample(4)}),
                                          LibraryVersions({FFmpegVersion::FFmpeg_4x,
                                                           VersionAVFormat(58),
                                                           VersionAVCodec(58),
                                                           VersionAVUtil(56),
                                                           VersionSwresample(3)}),
                                          LibraryVersions({FFmpegVersion::FFmpeg_3x,
                                                           VersionAVFormat(57),
                                                           VersionAVCodec(57),
                                                           VersionAVUtil(55),
                                                           VersionSwresample(2)}),
                                          LibraryVersions({FFmpegVersion::FFmpeg_2x,
                                                           VersionAVFormat(56),
                                                           VersionAVCodec(56),
                                                           VersionAVUtil(54),
                                                           VersionSwresample(1)})};

} // namespace ffmpeg
