/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "Types.h"
#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <Decoder.h>

namespace ffmpeg
{

std::string toString(int64_t timestamp, Rational timebase);

std::string to_string(const avcodec::AVPacketWrapper::Flags &flags);

enum class ConcatenationSymbol
{
  Comma,
  Newline
};

std::string to_string(const std::vector<std::string> &strings,
                      const ConcatenationSymbol concatenationSymbol = ConcatenationSymbol::Comma);

std::string to_string(const Decoder::State state);

} // namespace ffmpeg
