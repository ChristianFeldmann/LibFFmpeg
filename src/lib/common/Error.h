/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/EnumMapper.h>

#include <errno.h>
#include <optional>
#include <sstream>
#include <stdint.h>
#include <vector>

namespace libffmpeg
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

const EnumMapper<ReturnCode> ReturnCodeMapper({{ReturnCode::Ok, "Ok"},
                                               {ReturnCode::TryAgain, "TryAgain"},
                                               {ReturnCode::BSFNotFound, "BSFNotFound"},
                                               {ReturnCode::Bug, "Bug"},
                                               {ReturnCode::BufferTooSmall, "BufferTooSmall"},
                                               {ReturnCode::DecoderNotFound, "DecoderNotFound"},
                                               {ReturnCode::DemuxerNotFound, "DemuxerNotFound"},
                                               {ReturnCode::EncoderNotFound, "EncoderNotFound"},
                                               {ReturnCode::EndOfFile, "EndOfFile"},
                                               {ReturnCode::Exit, "Exit"},
                                               {ReturnCode::External, "External"},
                                               {ReturnCode::FilterNotFound, "FilterNotFound"},
                                               {ReturnCode::InvalidData, "InvalidData"},
                                               {ReturnCode::MuxerNotFound, "MuxerNotFound"},
                                               {ReturnCode::OptionNotFound, "OptionNotFound"},
                                               {ReturnCode::NotImplementedYet, "NotImplementedYet"},
                                               {ReturnCode::ProtocolNotFound, "ProtocolNotFound"},
                                               {ReturnCode::StreamNotFound, "StreamNotFound"},
                                               {ReturnCode::Experimental, "Experimental"},
                                               {ReturnCode::InputChanged, "InputChanged"},
                                               {ReturnCode::OutputChanged, "OutputChanged"},
                                               {ReturnCode::HttpBadRequest, "HttpBadRequest"},
                                               {ReturnCode::HttpUnauthorized, "HttpUnauthorized"},
                                               {ReturnCode::HttpForbidden, "HttpForbidden"},
                                               {ReturnCode::HttpNotFound, "HttpNotFound"},
                                               {ReturnCode::HttpOther4xx, "HttpOther4xx"},
                                               {ReturnCode::HttpServerError, "HttpServerError"},
                                               {ReturnCode::Unknown, "Unknown"}});

ReturnCode toReturnCode(const int returnValue);
int        toAVError(const ReturnCode returnCode);

} // namespace libffmpeg
