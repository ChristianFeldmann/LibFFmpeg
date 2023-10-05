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

#include "SwResampleFunctions.h"

#include "Functions.h"

namespace LibFFmpeg::functions
{

std::optional<SwResampleFunctions> tryBindSwResampleFunctionsFromLibrary(SharedLibraryLoader &lib,
                                                                         Log &                log)
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

} // namespace LibFFmpeg::functions
