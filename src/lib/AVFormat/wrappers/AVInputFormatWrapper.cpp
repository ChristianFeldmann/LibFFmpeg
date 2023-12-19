/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVInputFormatWrapper.h"

#include "AVInputFormatWrapperInternal.h"
#include <common/InternalTypes.h>

#include "CastFormatClasses.h"

#include <sstream>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVInputFormat;
using ffmpeg::internal::avformat::AVInputFormat_56;
using ffmpeg::internal::avformat::AVInputFormat_57;
using ffmpeg::internal::avformat::AVInputFormat_58;
using ffmpeg::internal::avformat::AVInputFormat_59;
using ffmpeg::internal::avformat::AVInputFormat_60;

} // namespace

std::string to_string(const AVInputFormatFlags &flags)
{
  std::ostringstream flagsString;
  if (flags.noFile)
    flagsString << "NoFile, ";
  if (flags.needNumber)
    flagsString << "NeedNumber, ";
  if (flags.experimental)
    flagsString << "Experimental, ";
  if (flags.showIDs)
    flagsString << "ShowIDs, ";
  if (flags.globalHeaders)
    flagsString << "GlobalHeaders, ";
  if (flags.noTiemstamps)
    flagsString << "NoTimestamp, ";
  if (flags.genericIndex)
    flagsString << "GenericIndex, ";
  if (flags.tsDiscont)
    flagsString << "TsDiscont, ";
  if (flags.variableFPS)
    flagsString << "VariableFPS, ";
  if (flags.noDimensions)
    flagsString << "NoDimensions, ";
  if (flags.noStreams)
    flagsString << "NoStreams, ";
  if (flags.noBinSearch)
    flagsString << "NoBinSearch, ";
  if (flags.noGenSearch)
    flagsString << "noGenSearch, ";
  if (flags.noByteSeek)
    flagsString << "noByteSeek, ";
  if (flags.allowFlush)
    flagsString << "AllowFlush, ";
  if (flags.tsNonStrict)
    flagsString << "TsNonStrict, ";
  if (flags.tsNegative)
    flagsString << "TsNegative, ";
  if (flags.seekToPTS)
    flagsString << "SeekToPTS, ";
  const auto fullString = flagsString.str();
  if (fullString.empty())
    return {};
  return fullString.substr(0, fullString.size() - 2);
}

bool operator==(const AVInputFormatFlags &lhs, const AVInputFormatFlags &rhs)
{
  return (lhs.noFile == rhs.noFile && lhs.needNumber == rhs.needNumber &&
          lhs.experimental == rhs.experimental && lhs.showIDs == rhs.showIDs &&
          lhs.globalHeaders == rhs.globalHeaders && lhs.noTiemstamps == rhs.noTiemstamps &&
          lhs.genericIndex == rhs.genericIndex && lhs.tsDiscont == rhs.tsDiscont &&
          lhs.variableFPS == rhs.variableFPS && lhs.noDimensions == rhs.noDimensions &&
          lhs.noStreams == rhs.noStreams && lhs.noBinSearch == rhs.noBinSearch &&
          lhs.noGenSearch == rhs.noGenSearch && lhs.noByteSeek == rhs.noByteSeek &&
          lhs.allowFlush == rhs.allowFlush && lhs.tsNonStrict == rhs.tsNonStrict &&
          lhs.tsNegative == rhs.tsNegative && lhs.seekToPTS == rhs.seekToPTS);
}

AVInputFormatWrapper::AVInputFormatWrapper(AVInputFormat                    *inputFormat,
                                           std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : inputFormat(inputFormat), ffmpegLibraries(ffmpegLibraries)
{
  if (inputFormat == nullptr)
    throw std::runtime_error("Provided input format pointer must not be null");
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
}

std::string AVInputFormatWrapper::getName() const
{
  const char *nameCStr{};
  CAST_AVFORMAT_GET_MEMBER(AVInputFormat, this->inputFormat, nameCStr, name);
  return nameCStr == nullptr ? "" : std::string(nameCStr);
}

std::string AVInputFormatWrapper::getLongName() const
{
  const char *longNameCStr{};
  CAST_AVFORMAT_GET_MEMBER(AVInputFormat, this->inputFormat, longNameCStr, long_name);
  return longNameCStr == nullptr ? "" : std::string(longNameCStr);
}

AVInputFormatFlags AVInputFormatWrapper::getFlags() const
{
  int rawFlags{};
  CAST_AVFORMAT_GET_MEMBER(AVInputFormat, this->inputFormat, rawFlags, flags);

  AVInputFormatFlags flags;
  flags.noFile        = (rawFlags & internal::avformat::AVFMT_NOFILE);
  flags.needNumber    = (rawFlags & internal::avformat::AVFMT_NEEDNUMBER);
  flags.experimental  = (rawFlags & internal::avformat::AVFMT_EXPERIMENTAL);
  flags.showIDs       = (rawFlags & internal::avformat::AVFMT_SHOW_IDS);
  flags.globalHeaders = (rawFlags & internal::avformat::AVFMT_GLOBALHEADER);
  flags.noTiemstamps  = (rawFlags & internal::avformat::AVFMT_NOTIMESTAMPS);
  flags.genericIndex  = (rawFlags & internal::avformat::AVFMT_GENERIC_INDEX);
  flags.tsDiscont     = (rawFlags & internal::avformat::AVFMT_TS_DISCONT);
  flags.variableFPS   = (rawFlags & internal::avformat::AVFMT_VARIABLE_FPS);
  flags.noDimensions  = (rawFlags & internal::avformat::AVFMT_NODIMENSIONS);
  flags.noStreams     = (rawFlags & internal::avformat::AVFMT_NOSTREAMS);
  flags.noBinSearch   = (rawFlags & internal::avformat::AVFMT_NOBINSEARCH);
  flags.noGenSearch   = (rawFlags & internal::avformat::AVFMT_NOGENSEARCH);
  flags.noByteSeek    = (rawFlags & internal::avformat::AVFMT_NO_BYTE_SEEK);
  flags.allowFlush    = (rawFlags & internal::avformat::AVFMT_ALLOW_FLUSH);
  flags.tsNonStrict   = (rawFlags & internal::avformat::AVFMT_TS_NONSTRICT);
  flags.tsNegative    = (rawFlags & internal::avformat::AVFMT_TS_NEGATIVE);
  flags.seekToPTS     = (rawFlags & internal::avformat::AVFMT_SEEK_TO_PTS);
  return flags;
}

std::string AVInputFormatWrapper::getExtensions() const
{
  const char *extensionsCstr{};
  CAST_AVFORMAT_GET_MEMBER(AVInputFormat, this->inputFormat, extensionsCstr, extensions);
  return extensionsCstr == nullptr ? "" : std::string(extensionsCstr);
}

std::string AVInputFormatWrapper::getMimeType() const
{
  const char *mimeTypeCStr{};
  CAST_AVFORMAT_GET_MEMBER(AVInputFormat, this->inputFormat, mimeTypeCStr, mime_type);
  return mimeTypeCStr == nullptr ? "" : std::string(mimeTypeCStr);
}

} // namespace ffmpeg::avformat
