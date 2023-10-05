/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFormatFunctions.h"

#include "Functions.h"

namespace LibFFmpeg::functions
{

std::optional<AvFormatFunctions> tryBindFunctionsFromLibrary(SharedLibraryLoader &lib, Log &log)
{
  if (!lib)
  {
    log.push_back("Binding of avFormat functions failed. Library is not loaded.");
    return {};
  }

  AvFormatFunctions functions;

  lib.tryResolveFunction(functions.avformat_version, "avformat_version");
  lib.tryResolveFunction(functions.av_register_all, "av_register_all");
  lib.tryResolveFunction(functions.avformat_open_input, "avformat_open_input");
  lib.tryResolveFunction(functions.avformat_close_input, "avformat_close_input");
  lib.tryResolveFunction(functions.avformat_find_stream_info, "avformat_find_stream_info");
  lib.tryResolveFunction(functions.av_read_frame, "av_read_frame");
  lib.tryResolveFunction(functions.av_seek_frame, "av_seek_frame");

  std::vector<std::string> missingFunctions;

  checkForMissingFunctionAndLog(
      functions.avformat_version, "avformat_version", missingFunctions, log);

  if (!functions.avformat_version)
  {
    log.push_back("Binding avFormat functions failed. Missing function avformat_version");
    return {};
  }

  const auto version = Version::fromFFmpegVersion(functions.avformat_version());

  if (version.major < 59)
    checkForMissingFunctionAndLog(
        functions.av_register_all, "av_register_all", missingFunctions, log);

  checkForMissingFunctionAndLog(
      functions.avformat_open_input, "avformat_open_input", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avformat_close_input, "avformat_close_input", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avformat_find_stream_info, "avformat_find_stream_info", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_read_frame, "av_read_frame", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_seek_frame, "av_seek_frame", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avformat_version, "avformat_version", missingFunctions, log);

  if (!missingFunctions.empty())
  {
    log.push_back("Binding avFormat functions failed. Missing functions: " +
                  to_string(missingFunctions));
    return {};
  }

  log.push_back("Binding of avFormat functions successful.");
  return functions;
}

} // namespace LibFFmpeg::functions
