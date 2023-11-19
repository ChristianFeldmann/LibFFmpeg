/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVInputFormatWrapper.h"

#include "CastFormatClasses.h"

namespace ffmpeg::avformat
{

namespace
{

struct AVInputFormat_56
{
  const char *                    name;
  const char *                    long_name;
  int                             flags;
  const char *                    extensions;
  const struct AVCodecTag *const *codec_tag;
  const AVClass *                 priv_class;
  const char *                    mime_type;

  // There is more but it is not part of the public ABI
};

typedef AVInputFormat_56 AVInputFormat_57;
typedef AVInputFormat_56 AVInputFormat_58;
typedef AVInputFormat_56 AVInputFormat_59;
typedef AVInputFormat_56 AVInputFormat_60;

constexpr auto AVFMT_NOFILE        = 0x0001;
constexpr auto AVFMT_NEEDNUMBER    = 0x0002;
constexpr auto AVFMT_EXPERIMENTAL  = 0x0004;
constexpr auto AVFMT_SHOW_IDS      = 0x0008;
constexpr auto AVFMT_GLOBALHEADER  = 0x0040;
constexpr auto AVFMT_NOTIMESTAMPS  = 0x0080;
constexpr auto AVFMT_GENERIC_INDEX = 0x0100;
constexpr auto AVFMT_TS_DISCONT    = 0x0200;
constexpr auto AVFMT_VARIABLE_FPS  = 0x0400;
constexpr auto AVFMT_NODIMENSIONS  = 0x0800;
constexpr auto AVFMT_NOSTREAMS     = 0x1000;
constexpr auto AVFMT_NOBINSEARCH   = 0x2000;
constexpr auto AVFMT_NOGENSEARCH   = 0x4000;
constexpr auto AVFMT_NO_BYTE_SEEK  = 0x8000;
constexpr auto AVFMT_ALLOW_FLUSH   = 0x10000;
constexpr auto AVFMT_TS_NONSTRICT  = 0x20000;
constexpr auto AVFMT_TS_NEGATIVE   = 0x40000;
constexpr auto AVFMT_SEEK_TO_PTS   = 0x4000000;

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

AVInputFormatWrapper::AVInputFormatWrapper(
    AVInputFormat *inputFormat, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : inputFormat(inputFormat), librariesInterface(librariesInterface)
{
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
  flags.noFile        = (rawFlags & AVFMT_NOFILE);
  flags.needNumber    = (rawFlags & AVFMT_NEEDNUMBER);
  flags.experimental  = (rawFlags & AVFMT_EXPERIMENTAL);
  flags.showIDs       = (rawFlags & AVFMT_SHOW_IDS);
  flags.globalHeaders = (rawFlags & AVFMT_GLOBALHEADER);
  flags.noTiemstamps  = (rawFlags & AVFMT_NOTIMESTAMPS);
  flags.genericIndex  = (rawFlags & AVFMT_GENERIC_INDEX);
  flags.tsDiscont     = (rawFlags & AVFMT_TS_DISCONT);
  flags.variableFPS   = (rawFlags & AVFMT_VARIABLE_FPS);
  flags.noDimensions  = (rawFlags & AVFMT_NODIMENSIONS);
  flags.noStreams     = (rawFlags & AVFMT_NOSTREAMS);
  flags.noBinSearch   = (rawFlags & AVFMT_NOBINSEARCH);
  flags.noGenSearch   = (rawFlags & AVFMT_NOGENSEARCH);
  flags.noByteSeek    = (rawFlags & AVFMT_NO_BYTE_SEEK);
  flags.allowFlush    = (rawFlags & AVFMT_ALLOW_FLUSH);
  flags.tsNonStrict   = (rawFlags & AVFMT_TS_NONSTRICT);
  flags.tsNegative    = (rawFlags & AVFMT_TS_NEGATIVE);
  flags.seekToPTS     = (rawFlags & AVFMT_SEEK_TO_PTS);
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
