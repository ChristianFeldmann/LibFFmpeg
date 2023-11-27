/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <string>
#include <vector>

namespace ffmpeg
{

enum class ConcatenationSymbol
{
  Comma,
  Newline
};

std::string to_string(const std::vector<std::string> &strings,
                      const ConcatenationSymbol concatenationSymbol = ConcatenationSymbol::Comma);

} // namespace ffmpeg
