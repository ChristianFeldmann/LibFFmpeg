/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Functions.h"

#include <common/Formatting.h>

namespace ffmpeg::internal::functions
{

std::string logMissingFunctionsAndGetErrorMessage(const std::vector<std::string> &missingFunctions,
                                                  const std::string               libraryName,
                                                  const LoggingFunction &         log)
{
  if (missingFunctions.empty())
    return {};

  const auto errorMessage = "Binding " + libraryName +
                            " functions failed. Missing functions: " + to_string(missingFunctions);
  log(LogLevel::Debug, errorMessage);
  return errorMessage;
}

} // namespace ffmpeg::internal::functions
