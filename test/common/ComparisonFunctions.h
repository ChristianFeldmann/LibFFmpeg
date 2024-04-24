/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/wrappers/AVFrameWrapper.h>

#include <string>
#include <vector>

namespace libffmpeg
{

bool         areEqual(const std::vector<std::string> &lhs, const std::vector<std::string> &rhs);
std::int64_t calculateFrameDataHash(const avutil::AVFrameWrapper &frame);

} // namespace libffmpeg
