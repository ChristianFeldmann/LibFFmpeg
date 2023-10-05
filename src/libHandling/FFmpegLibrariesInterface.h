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

#pragma once

#include <common/Expected.h>
#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/SharedLibraryLoader.h>
#include <libHandling/libraryFunctions/AVFormatFunctions.h>
#include <libHandling/libraryFunctions/AvCodecFunctions.h>
#include <libHandling/libraryFunctions/AvUtilFunctions.h>
#include <libHandling/libraryFunctions/SwResampleFunctions.h>

#include <filesystem>

namespace LibFFmpeg
{

struct LibraryInfo
{
  std::string           name;
  std::filesystem::path path;
  std::string           version;
};

class FFmpegLibrariesInterface
{
public:
  FFmpegLibrariesInterface()  = default;
  ~FFmpegLibrariesInterface() = default;

  using LoadingResultAndLog = std::pair<bool, Log>;
  LoadingResultAndLog tryLoadFFmpegLibrariesInPath(const std::filesystem::path &path);

  std::vector<LibraryInfo> getLibrariesInfo() const;

  functions::AvFormatFunctions   avformat{};
  functions::AvCodecFunctions    avcodec{};
  functions::AvUtilFunctions     avutil{};
  functions::SwResampleFunctions swresample{};

private:
  bool
  tryLoadLibrariesBindFunctionsAndCheckVersions(const std::filesystem::path &absoluteDirectoryPath,
                                                const LibraryVersions &      libraryVersions,
                                                Log &                        log);

  void unloadAllLibraries();

  SharedLibraryLoader libAvutil;
  SharedLibraryLoader libSwresample;
  SharedLibraryLoader libAvcodec;
  SharedLibraryLoader libAvformat;

  LibraryVersions libraryVersions{};

  static std::string logListFFmpeg;
  static void        avLogCallback(void *ptr, int level, const char *fmt, va_list vargs);

  friend class FFmpegLibrariesInterfaceBuilder;
};

} // namespace LibFFmpeg
