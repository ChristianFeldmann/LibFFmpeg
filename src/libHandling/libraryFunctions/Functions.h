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

#pragma once

#include <common/Expected.h>
#include <common/FFMpegLibrariesTypes.h>

#include <sstream>
#include <string>
#include <vector>

namespace LibFFmpeg::functions
{

using LoadingResult       = tl::expected<void, std::string>;
using LoadingResultAndLog = std::pair<LoadingResult, Log>;

template <typename T>
void checkForMissingFunctionAndLog(std::function<T> &        function,
                                   std::string               name,
                                   std::vector<std::string> &missingFunctions,
                                   Log &                     log)
{
  if (function)
    log.push_back("Successfully resolved function " + name);
  else
  {
    log.push_back("Failed to resolved function " + name);
    missingFunctions.push_back(name);
  }
}

std::string to_string(std::vector<std::string> strings);

std::string logMissingFunctionsAndGetErrorMessage(const std::vector<std::string> &missingFunctions,
                                                  const std::string               libraryName,
                                                  Log &                           log);

} // namespace LibFFmpeg::functions
