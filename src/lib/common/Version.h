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

namespace libffmpeg
{

struct Version
{
  constexpr Version() = default;
  constexpr Version(const int major) : major(major) {}

  bool operator!=(const Version &other) const;

  [[nodiscard]] std::string   toString() const;
  [[nodiscard]] std::ostream &operator<<(std::ostream &stream) const;

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
  FFmpeg_7x,
  FFmpeg_8x
};

const EnumMapper<FFmpegVersion>
    ffmpegVersionMapper({{FFmpegVersion::FFmpeg_2x, "2.x", "FFmpeg 2.x"},
                         {FFmpegVersion::FFmpeg_3x, "3.x", "FFmpeg 3.x"},
                         {FFmpegVersion::FFmpeg_4x, "4.x", "FFmpeg 4.x"},
                         {FFmpegVersion::FFmpeg_5x, "5.x", "FFmpeg 5.x"},
                         {FFmpegVersion::FFmpeg_6x, "6.x", "FFmpeg 6.x"},
                         {FFmpegVersion::FFmpeg_7x, "7.x", "FFmpeg 7.x"},
                         {FFmpegVersion::FFmpeg_8x, "8.x", "FFmpeg 8.x"}});

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
constexpr auto SupportedFFmpegVersions = {
    LibraryVersions({.ffmpegVersion = FFmpegVersion::FFmpeg_8x,
                     .avformat      = VersionAVFormat(62),
                     .avcodec       = VersionAVCodec(62),
                     .avutil        = VersionAVUtil(60),
                     .swresample    = VersionSwresample(6)}),
    LibraryVersions({.ffmpegVersion = FFmpegVersion::FFmpeg_7x,
                     .avformat      = VersionAVFormat(61),
                     .avcodec       = VersionAVCodec(61),
                     .avutil        = VersionAVUtil(59),
                     .swresample    = VersionSwresample(5)}),
    LibraryVersions({.ffmpegVersion = FFmpegVersion::FFmpeg_6x,
                     .avformat      = VersionAVFormat(60),
                     .avcodec       = VersionAVCodec(60),
                     .avutil        = VersionAVUtil(58),
                     .swresample    = VersionSwresample(4)}),
    LibraryVersions({.ffmpegVersion = FFmpegVersion::FFmpeg_5x,
                     .avformat      = VersionAVFormat(59),
                     .avcodec       = VersionAVCodec(59),
                     .avutil        = VersionAVUtil(57),
                     .swresample    = VersionSwresample(4)}),
    LibraryVersions({.ffmpegVersion = FFmpegVersion::FFmpeg_4x,
                     .avformat      = VersionAVFormat(58),
                     .avcodec       = VersionAVCodec(58),
                     .avutil        = VersionAVUtil(56),
                     .swresample    = VersionSwresample(3)}),
    LibraryVersions({.ffmpegVersion = FFmpegVersion::FFmpeg_3x,
                     .avformat      = VersionAVFormat(57),
                     .avcodec       = VersionAVCodec(57),
                     .avutil        = VersionAVUtil(55),
                     .swresample    = VersionSwresample(2)}),
    LibraryVersions({.ffmpegVersion = FFmpegVersion::FFmpeg_2x,
                     .avformat      = VersionAVFormat(56),
                     .avcodec       = VersionAVCodec(56),
                     .avutil        = VersionAVUtil(54),
                     .swresample    = VersionSwresample(1)})};

} // namespace libffmpeg
