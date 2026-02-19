/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Formatting.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace libffmpeg
{

std::string to_string(int64_t timestamp, Rational timebase)
{
  auto absolutSeconds = static_cast<double>(timestamp) * static_cast<double>(timebase.numerator) /
                        static_cast<double>(timebase.denominator);

  constexpr auto secondsPerHour   = 60 * 60;
  constexpr auto secondsPerMinute = 60;

  const auto hours = static_cast<int>(absolutSeconds / secondsPerHour);
  absolutSeconds -= hours * secondsPerHour;

  const auto minutes = static_cast<int>(absolutSeconds / secondsPerMinute);
  absolutSeconds -= minutes * secondsPerMinute;

  const auto seconds = static_cast<int>(absolutSeconds);
  absolutSeconds -= seconds;

  constexpr auto millisecondsPerSecond = 1000;
  auto           milliseconds          = static_cast<int>(absolutSeconds * millisecondsPerSecond);

  std::ostringstream stream;
  stream << std::setfill('0') << std::setw(2) << hours << ":";
  stream << std::setfill('0') << std::setw(2) << minutes << ":";
  stream << std::setfill('0') << std::setw(2) << seconds << ".";
  stream << std::setfill('0') << std::setw(3) << milliseconds << ":";

  return stream.str();
}

std::string to_string(const avcodec::AVPacketWrapper::Flags &flags)
{
  std::string flagsString;
  if (flags.keyframe)
    flagsString += "Keyframe,";
  if (flags.corrupt)
    flagsString += "Corrupt,";
  if (flags.discard)
    flagsString += "Discard";
  if (!flagsString.empty())
    flagsString.pop_back();
  return flagsString;
}

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

std::string to_string(const Decoder::State state)
{
  switch (state)
  {
  case Decoder::State::NotOpened:
    return "NotOpened";
  case Decoder::State::NeedsMoreData:
    return "NeedsMoreData";
  case Decoder::State::RetrieveFrames:
    return "RetrieveFrames";
  case Decoder::State::EndOfBitstream:
    return "EndOfBitstream";
  case Decoder::State::Error:
    return "Error";
  default:
    return "";
  }
}

std::string to_string(const LogLevel logLevel)
{
  switch (logLevel)
  {
  case LogLevel::Debug:
    return "Debug";
  case LogLevel::Info:
    return "Info";
  case LogLevel::Warning:
    return "Warning";
  case LogLevel::Error:
    return "Error";

  default:
    return "";
  }
}

} // namespace libffmpeg
