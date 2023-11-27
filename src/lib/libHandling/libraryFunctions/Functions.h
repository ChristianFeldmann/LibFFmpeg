/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Types.h>

#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace ffmpeg::internal::functions
{

template <typename T>
void checkForMissingFunctionAndLog(const std::function<T>   &function,
                                   const std::string        &name,
                                   std::vector<std::string> &missingFunctions,
                                   Log                      &log)
{
  if (function)
    log.push_back("Successfully resolved function " + name);
  else
  {
    log.push_back("Failed to resolved function " + name);
    missingFunctions.push_back(name);
  }
}

std::string logMissingFunctionsAndGetErrorMessage(const std::vector<std::string> &missingFunctions,
                                                  const std::string               libraryName,
                                                  Log                            &log);

} // namespace ffmpeg::internal::functions
