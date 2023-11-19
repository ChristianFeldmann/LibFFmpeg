/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Functions.h"

namespace ffmpeg::functions
{

std::string to_string(const std::vector<std::string> &strings,
                      const ConcatenationSymbol       concatenationSymbol)
{
  std::ostringstream stream;
  for (auto it = strings.begin(); it != strings.end(); it++)
  {
    if (it != strings.begin())
    {
      if (concatenationSymbol == ConcatenationSymbol::Comma)
        stream << ", ";
      else if (concatenationSymbol == ConcatenationSymbol::Newline)
        stream << "\n";
    }
    stream << (*it);
  }
  return stream.str();
}

std::string logMissingFunctionsAndGetErrorMessage(const std::vector<std::string> &missingFunctions,
                                                  const std::string               libraryName,
                                                  Log &                           log)
{
  if (missingFunctions.empty())
    return {};

  const auto errorMessage = "Binding " + libraryName +
                            " functions failed. Missing functions: " + to_string(missingFunctions);
  log.push_back(errorMessage);
  return errorMessage;
}

} // namespace ffmpeg::functions
