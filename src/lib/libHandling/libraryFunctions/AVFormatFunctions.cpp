/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFormatFunctions.h"

#include <common/Formatting.h>
#include <common/Version.h>

#include "Functions.h"

namespace libffmpeg::internal::functions
{

std::optional<AvFormatFunctions> tryBindAVFormatFunctionsFromLibrary(const SharedLibraryLoader &lib,
                                                                     const LoggingFunction     &log)
{
  if (!lib)
  {
    log(LogLevel::Error, "Binding of avFormat functions failed. Library is not loaded.");
    return {};
  }

  AvFormatFunctions functions;

  lib.tryResolveFunction(functions.avformat_version, "avformat_version");
  lib.tryResolveFunction(functions.av_register_all, "av_register_all");
  lib.tryResolveFunction(functions.avformat_open_input, "avformat_open_input");
  lib.tryResolveFunction(functions.avformat_close_input, "avformat_close_input");
  lib.tryResolveFunction(functions.avformat_alloc_context, "avformat_alloc_context");
  lib.tryResolveFunction(functions.avformat_find_stream_info, "avformat_find_stream_info");
  lib.tryResolveFunction(functions.av_read_frame, "av_read_frame");
  lib.tryResolveFunction(functions.av_seek_frame, "av_seek_frame");
  lib.tryResolveFunction(functions.avio_alloc_context, "avio_alloc_context");
  lib.tryResolveFunction(functions.avio_context_free, "avio_context_free");

  std::vector<std::string> missingFunctions;

  checkForMissingFunctionAndLog(
      functions.avformat_version, "avformat_version", missingFunctions, log);

  if (!functions.avformat_version)
  {
    log(LogLevel::Debug, "Binding avFormat functions failed. Missing function avformat_version");
    return {};
  }

  const auto version = Version::fromFFmpegVersion(functions.avformat_version());

  checkForMissingFunctionAndLog(
      functions.avformat_version, "avformat_version", missingFunctions, log);
  if (version.major < 59)
    checkForMissingFunctionAndLog(
        functions.av_register_all, "av_register_all", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avformat_open_input, "avformat_open_input", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avformat_close_input, "avformat_close_input", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avformat_alloc_context, "avformat_alloc_context", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avformat_find_stream_info, "avformat_find_stream_info", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_read_frame, "av_read_frame", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_seek_frame, "av_seek_frame", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avio_alloc_context, "avio_alloc_context", missingFunctions, log);
  if (version.major > 56)
    checkForMissingFunctionAndLog(
      functions.avio_context_free, "avio_context_free", missingFunctions, log);

  if (!missingFunctions.empty())
  {
    log(LogLevel::Info,
        "Binding avFormat functions failed. Missing functions: " + to_string(missingFunctions));
    return {};
  }

  log(LogLevel::Debug, "Binding of avFormat functions successful.");
  return functions;
}

} // namespace libffmpeg::internal::functions
