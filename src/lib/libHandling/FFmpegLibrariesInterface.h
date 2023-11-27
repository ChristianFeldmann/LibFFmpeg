/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Types.h>
#include <common/Version.h>
#include <libHandling/SharedLibraryLoader.h>
#include <libHandling/libraryFunctions/AVFormatFunctions.h>
#include <libHandling/libraryFunctions/AvCodecFunctions.h>
#include <libHandling/libraryFunctions/AvUtilFunctions.h>
#include <libHandling/libraryFunctions/SwResampleFunctions.h>

#include <filesystem>

namespace ffmpeg
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

  ResultAndLog tryLoadFFmpegLibrariesInPath(const std::filesystem::path &path);

  std::vector<LibraryInfo> getLibrariesInfo() const;
  LibraryVersions          getLibrariesVersion() const { return this->libraryVersions; }

  std::string_view getLogList() const { return this->logListFFmpeg; }

  internal::functions::AvFormatFunctions   avformat{};
  internal::functions::AvCodecFunctions    avcodec{};
  internal::functions::AvUtilFunctions     avutil{};
  internal::functions::SwResampleFunctions swresample{};

private:
  bool
  tryLoadLibrariesBindFunctionsAndCheckVersions(const std::filesystem::path &absoluteDirectoryPath,
                                                const LibraryVersions       &libraryVersions,
                                                Log                         &log);

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

} // namespace ffmpeg
