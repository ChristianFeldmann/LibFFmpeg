/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Logging.h>
#include <common/Types.h>

#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace ffmpeg::internal::functions
{

template <typename T>
void checkForMissingFunctionAndLog(const std::function<T> &  function,
                                   const std::string &       name,
                                   std::vector<std::string> &missingFunctions,
                                   const LoggingFunction &   log)
{
  if (function)
    log(LogLevel::Debug, "Successfully resolved function " + name);
  else
  {
    log(LogLevel::Debug, "Failed to resolved function " + name);
    missingFunctions.push_back(name);
  }
}

std::string logMissingFunctionsAndGetErrorMessage(const std::vector<std::string> &missingFunctions,
                                                  const std::string               libraryName,
                                                  const LoggingFunction &         log);

} // namespace ffmpeg::internal::functions
