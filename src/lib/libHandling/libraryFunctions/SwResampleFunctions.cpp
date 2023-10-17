/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "SwResampleFunctions.h"

#include "Functions.h"

namespace ffmpeg::functions
{

std::optional<SwResampleFunctions>
tryBindSwResampleFunctionsFromLibrary(const SharedLibraryLoader &lib, Log &log)
{
  if (!lib)
  {
    log.push_back("Binding of swResample functions failed. Library is not loaded.");
    return {};
  }

  SwResampleFunctions functions;

  lib.tryResolveFunction(functions.swresample_version, "swresample_version");

  std::vector<std::string> missingFunctions;

  checkForMissingFunctionAndLog(
      functions.swresample_version, "swresample_version", missingFunctions, log);

  if (!missingFunctions.empty())
  {
    log.push_back("Binding swResample functions failed. Missing functions: " +
                  to_string(missingFunctions));
    return {};
  }

  log.push_back("Binding of swPresample functions successful.");
  return functions;
}

} // namespace ffmpeg::functions
