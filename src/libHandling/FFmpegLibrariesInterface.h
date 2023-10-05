/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
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
