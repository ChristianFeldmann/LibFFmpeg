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
