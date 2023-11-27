/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <errno.h>
#include <optional>
#include <sstream>
#include <stdint.h>
#include <vector>

namespace ffmpeg
{

enum class ReturnCode
{
  Ok,
  TryAgain,
  BSFNotFound,
  Bug,
  BufferTooSmall,
  DecoderNotFound,
  DemuxerNotFound,
  EncoderNotFound,
  EndOfFile,
  Exit,
  External,
  FilterNotFound,
  InvalidData,
  MuxerNotFound,
  OptionNotFound,
  NotImplementedYet,
  ProtocolNotFound,
  StreamNotFound,
  Experimental,
  InputChanged,
  OutputChanged,
  HttpBadRequest,
  HttpUnauthorized,
  HttpForbidden,
  HttpNotFound,
  HttpOther4xx,
  HttpServerError,
  Unknown
};

ReturnCode toReturnCode(const int returnValue);

} // namespace ffmpeg
