/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <wrappers/AVUtil/AVFrameWrapper.h>

#include <string>
#include <vector>

namespace ffmpeg
{

bool        areEqual(const std::vector<std::string> &lhs, const std::vector<std::string> &rhs);
std::size_t calculateFrameDataHash(const avutil::AVFrameWrapper &frame);

} // namespace ffmpeg
