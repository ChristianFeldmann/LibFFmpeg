/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Logging.h>
#include <common/Types.h>
#include <common/Version.h>
#include <libHandling/libraryFunctions/AVFormatFunctions.h>
#include <libHandling/libraryFunctions/AvCodecFunctions.h>
#include <libHandling/libraryFunctions/AvUtilFunctions.h>
#include <libHandling/libraryFunctions/SwResampleFunctions.h>

#include <filesystem>
#include <string>

namespace libffmpeg
{

struct LibraryInfo
{
  std::string name;
  Path        path;
  std::string version;
};

class IFFmpegLibraries
{
public:
  virtual ~IFFmpegLibraries(){};

  virtual std::vector<LibraryInfo> getLibrariesInfo() const    = 0;
  virtual LibraryVersions          getLibrariesVersion() const = 0;

  virtual void log(const LogLevel logLevel, const std::string &message) const = 0;

  internal::functions::AvFormatFunctions   avformat{};
  internal::functions::AvCodecFunctions    avcodec{};
  internal::functions::AvUtilFunctions     avutil{};
  internal::functions::SwResampleFunctions swresample{};
};

} // namespace libffmpeg
