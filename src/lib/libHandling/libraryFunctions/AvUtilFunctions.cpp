/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AvUtilFunctions.h"

#include <common/Formatting.h>
#include <common/Version.h>

#include "Functions.h"

namespace libffmpeg::internal::functions
{

std::optional<AvUtilFunctions> tryBindAVUtilFunctionsFromLibrary(const SharedLibraryLoader &lib,
                                                                 const LoggingFunction &    log)
{
  if (!lib)
  {
    log(LogLevel::Error, "Binding of avUtil functions failed. Library is not loaded.");
    return {};
  }

  AvUtilFunctions functions;

  lib.tryResolveFunction(functions.avutil_version, "avutil_version");
  lib.tryResolveFunction(functions.av_frame_alloc, "av_frame_alloc");
  lib.tryResolveFunction(functions.av_frame_free, "av_frame_free");
  lib.tryResolveFunction(functions.av_mallocz, "av_mallocz");
  lib.tryResolveFunction(functions.av_freep, "av_freep");
  lib.tryResolveFunction(functions.av_dict_set, "av_dict_set");
  lib.tryResolveFunction(functions.av_dict_get, "av_dict_get");
  lib.tryResolveFunction(functions.av_frame_get_side_data, "av_frame_get_side_data");
  lib.tryResolveFunction(functions.av_frame_get_metadata, "av_frame_get_metadata");
  lib.tryResolveFunction(functions.av_log_set_callback, "av_log_set_callback");
  lib.tryResolveFunction(functions.av_log_default_callback, "av_log_default_callback");
  lib.tryResolveFunction(functions.av_log_set_level, "av_log_set_level");
  lib.tryResolveFunction(functions.av_pix_fmt_desc_get, "av_pix_fmt_desc_get");
  lib.tryResolveFunction(functions.av_pix_fmt_desc_next, "av_pix_fmt_desc_next");
  lib.tryResolveFunction(functions.av_pix_fmt_desc_get_id, "av_pix_fmt_desc_get_id");

  std::vector<std::string> missingFunctions;

  checkForMissingFunctionAndLog(functions.avutil_version, "avutil_version", missingFunctions, log);
  if (!functions.avutil_version)
  {
    log(LogLevel::Error, "Binding avutil functions failed.Missing function avutil_version");
    return {};
  }

  const auto version = Version::fromFFmpegVersion(functions.avutil_version());

  checkForMissingFunctionAndLog(functions.av_frame_alloc, "av_frame_alloc", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_frame_free, "av_frame_free", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_frame_free, "av_frame_free", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_mallocz, "av_mallocz", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_freep, "av_freep", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_dict_set, "av_dict_set", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_dict_get, "av_dict_get", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_frame_get_side_data, "av_frame_get_side_data", missingFunctions, log);

  if (version.major < 57)
    checkForMissingFunctionAndLog(
        functions.av_frame_get_metadata, "av_frame_get_metadata", missingFunctions, log);

  checkForMissingFunctionAndLog(
      functions.av_log_set_callback, "av_log_set_callback", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_log_default_callback, "av_log_default_callback", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_log_set_level, "av_log_set_level", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_pix_fmt_desc_get, "av_pix_fmt_desc_get", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_pix_fmt_desc_next, "av_pix_fmt_desc_next", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_pix_fmt_desc_get_id, "av_pix_fmt_desc_get_id", missingFunctions, log);

  if (!missingFunctions.empty())
  {
    log(LogLevel::Debug,
        "Binding avUtil functions failed. Missing functions: " + to_string(missingFunctions));
    return {};
  }

  log(LogLevel::Debug, "Binding of avUtil functions successful.");
  return functions;
}

} // namespace libffmpeg::internal::functions
