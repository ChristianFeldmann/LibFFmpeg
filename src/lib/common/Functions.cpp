/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Functions.h"

#include <sstream>

namespace ffmpeg
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

} // namespace ffmpeg
