/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Functions.h"

namespace LibFFmpeg::functions
{

std::string to_string(std::vector<std::string> strings)
{
  std::ostringstream stream;
  for (auto it = strings.begin(); it != strings.end(); it++)
  {
    if (it != strings.begin())
      stream << ", ";
    stream << (*it);
  }
  return stream.str();
}

std::string logMissingFunctionsAndGetErrorMessage(const std::vector<std::string> &missingFunctions,
                                                  const std::string               libraryName,
                                                  Log &                           log)
{
  assert(!missingFunctions.empty());

  const auto errorMessage = "Binding " + libraryName +
                            " functions failed. Missing functions: " + to_string(missingFunctions);
  log.push_back(errorMessage);
  return errorMessage;
}

} // namespace LibFFmpeg::functions
