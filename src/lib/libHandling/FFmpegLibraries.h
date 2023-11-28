/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "IFFmpegLibraries.h"

#include <libHandling/SharedLibraryLoader.h>

namespace ffmpeg
{

class FFmpegLibraries : public IFFmpegLibraries
{
public:
  FFmpegLibraries()  = default;
  ~FFmpegLibraries() = default;

  std::vector<LibraryInfo> getLibrariesInfo() const override;
  LibraryVersions          getLibrariesVersion() const override { return this->libraryVersions; }

  std::string_view getLogList() const override { return this->logListFFmpeg; }

private:
  ResultAndLog tryLoadFFmpegLibrariesInPath(const std::filesystem::path &path);

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

  friend class FFmpegLibrariesBuilder;
};

} // namespace ffmpeg
