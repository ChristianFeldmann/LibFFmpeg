/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Demuxer.h>
#include <common/Formatting.h>
#include <libHandling/FFmpegLibrariesBuilder.h>

#include <cstddef>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace libffmpeg;

void showUsage()
{
  std::cout << "FileProber usage:\n";
  std::cout << "  FileProber <filename> [options]\n";
  std::cout << "\n";
  std::cout << "Options:\n";
  std::cout << "  -showAllPackets   Print all parsed packets\n";
  std::cout << "  -loglevelDebug    Set log level to debug output\n";
  std::cout << "  -loglevelInfo     Set log level to info output\n";
  std::cout << "  -loglevelWarning  Set log level to warning output\n";
  std::cout << "  -libPath          Set a search path for the ffmpeg libraries\n";
}

std::string to_string(const avcodec::CodecDescriptorProperties &properties)
{
  std::string str;
  if (properties.intraOnly)
    str += "Intra Only, ";
  if (properties.lossy)
    str += "Lossy, ";
  if (properties.lossless)
    str += "Lossless, ";
  if (properties.reorder)
    str += "Reorder, ";
  if (properties.bitmapSub)
    str += "Bitmap Subtitles, ";
  if (properties.textSub)
    str += "Text Subtitles, ";

  if (str.empty())
    return {};
  return str.substr(0, str.length() - 2);
}

std::string to_string(const Rational rational)
{
  return std::to_string(rational.numerator) + "/" + std::to_string(rational.denominator);
}

std::string to_string(const Size size)
{
  return std::to_string(size.width) + "x" + std::to_string(size.height);
}

std::string to_string(const ByteVector &bytes)
{
  std::ostringstream stream;
  for (const auto &byte : bytes)
    stream << "0x" << std::hex << std::to_integer<int>(byte) << " ";
  return stream.str();
}

template <typename T1, typename T2> std::string to_string(const std::map<T1, T2> &map)
{
  std::ostringstream stream;
  auto               isFirst = false;
  for (const auto [key, value] : map)
  {
    if (!isFirst)
      stream << ", ";
    stream << "[" << key << ", " << value << "]";
    isFirst = false;
  }
  return stream.str();
}

struct Settings
{
  std::string           filename{};
  bool                  showPackets{};
  std::filesystem::path libraryPath{};
  libffmpeg::LogLevel   logLevel{libffmpeg::LogLevel::Error};
};

bool checkIfFileExists(std::string_view filename)
{
  if (std::filesystem::exists(filename))
    return true;

  std::cout << "Given file " << filename << " not found.";
  return false;
}

std::optional<Settings> parseCommandLineArguments(int argc, char const *argv[])
{
  Settings settings;

  if (1 == argc)
  {
    std::cout << "Not enough arguemnts. File name must be given";
    return {};
  }

  for (int i = 1; i < argc; ++i)
  {
    const auto argument = std::string(argv[i]);

    if (1 == i)
    {
      if (checkIfFileExists(argument))
        settings.filename = argument;
      else
        return {};
    }
    if (argument == "-showAllPackets")
      settings.showPackets = true;
    if (argument == "-loglevelDebug")
      settings.logLevel = libffmpeg::LogLevel::Debug;
    if (argument == "-loglevelInfo")
      settings.logLevel = libffmpeg::LogLevel::Info;
    if (argument == "-loglevelWarning")
      settings.logLevel = libffmpeg::LogLevel::Warning;
    if (argument == "-libPath")
    {
      i++;
      if (i < argc)
      {
        const auto nextArgument = std::string(argv[i]);
        const auto path         = std::filesystem::path(nextArgument);
        const auto fileStatus   = std::filesystem::status(path);
        if (fileStatus.type() == std::filesystem::file_type::not_found)
          std::cout << "The given library path " << nextArgument
                    << " could not be found. Ignoring the given path.";
        else
          settings.libraryPath = path;
      }
      else
      {
        std::cout << "Missing argument for parameter -libPath";
        return {};
      }
    }
  }
  return settings;
}

void loggingFunction(const LogLevel logLevel, const std::string &message)
{
  static std::map<LogLevel, std::string> LogLevelToName = {{LogLevel::Debug, "Debug"},
                                                           {LogLevel::Info, "Info"},
                                                           {LogLevel::Warning, "Warning"},
                                                           {LogLevel::Error, "Error"}};

  std::cout << "[" << LogLevelToName.at(logLevel) << "]" << message << "\n";
}

int main(int argc, char const *argv[])
{
  const auto settings = parseCommandLineArguments(argc, argv);
  if (!settings)
  {
    showUsage();
    return 1;
  }

  const auto loadingResult = FFmpegLibrariesBuilder()
                                 .withLoggingFunction(&loggingFunction, settings->logLevel)
                                 .tryLoadingOfLibraries();

  if (!loadingResult.ffmpegLibraries)
  {
    std::cout << "Error when loading libraries\n";
    return 1;
  }
  else
  {
    std::cout << "Successfully loaded ffmpeg libraries.\n";
    std::cout << "\nLibraries info:\n";
    for (const auto info : loadingResult.ffmpegLibraries->getLibrariesInfo())
    {
      std::cout << "  " << info.name << ":\n";
      std::cout << "    Path   : " << info.path << ":\n";
      std::cout << "    Version: " << info.version << ":\n";
    }
  }

  Demuxer demuxer(loadingResult.ffmpegLibraries);
  if (!demuxer.openFile(settings->filename))
  {
    std::cout << "Error when opening input file : " + settings->filename + "\n ";
    return 1;
  }
  std::cout << "Successfully opened file " + settings->filename + ".\n";

  const auto formatContext = demuxer.getFormatContext();
  const auto inputFormat   = formatContext->getInputFormat();

  std::cout << "\nFile info:\n";

  std::cout << "  Format:\n";
  std::cout << "    Name              : " << inputFormat.getName() << "\n";
  std::cout << "    Long Name         : " << inputFormat.getLongName() << "\n";
  std::cout << "    Flags             : " << to_string(inputFormat.getFlags()) << "\n";
  std::cout << "    Extensions        : " << inputFormat.getExtensions() << "\n";
  std::cout << "    Mime Type         : " << inputFormat.getMimeType() << "\n";
  std::cout << "    Start time        : " << formatContext->getStartTime() << "\n";
  std::cout << "    Duration          : " << formatContext->getDuration() << "\n";

  for (const auto &stream : formatContext->getStreams())
  {
    std::cout << "  Streams " << stream.getIndex() << ":\n";
    std::cout << "    Codec Type        : "
              << avutil::mediaTypeMapper.getName(stream.getCodecType()) << "\n";

    std::cout << "    Codec Description\n";
    if (const auto codecDescriptor = stream.getCodecDescriptor())
    {
      std::cout << "      Media Type      : "
                << avutil::mediaTypeMapper.getName(codecDescriptor->mediaType) << "\n";
      std::cout << "      Name            : " << codecDescriptor->codecName << "\n";
      std::cout << "      Long Name       : " << codecDescriptor->longName << "\n";
      std::cout << "      Properties      : " << to_string(codecDescriptor->properties) << "\n";
      std::cout << "      Mime Types      : " << to_string(codecDescriptor->mimeTypes) << "\n";
      std::cout << "      Profiles        : " << to_string(codecDescriptor->profiles) << "\n";

      std::cout << "    Average Framerate : " << to_string(stream.getAverageFrameRate()) << "\n";
      std::cout << "    Time Base         : " << to_string(stream.getTimeBase()) << "\n";
      std::cout << "    Frame Size        : " << to_string(stream.getFrameSize()) << "\n";
      std::cout << "    Colorspace        : "
                << avutil::colorSpaceMapper.getName(stream.getColorspace()) << " - "
                << avutil::colorSpaceMapper.getText(stream.getColorspace()) << "\n";
      std::cout << "    Pixel Format      : " << stream.getPixelFormat().name << "\n";
      std::cout << "    Extradata         : " << to_string(stream.getExtradata()) << "\n";
    }
  }

  struct StreamCounters
  {
    int                packetCount{0};
    std::map<int, int> packetCountPerDuration{};
    std::map<int, int> packetCountPerPtsIncrease{};
    std::map<int, int> packetCountPerDtsIncrease{};
  };

  std::map<int, StreamCounters> streamPacketCounters;
  int                           packetIndex = 0;
  std::map<int, int64_t>        lastDTSPerStream;
  std::map<int, int64_t>        lastPTSPerStream;

  while (auto packet = demuxer.getNextPacket())
  {
    const auto streamIndex = packet->getStreamIndex();

    if (lastDTSPerStream.count(streamIndex) > 0 && lastPTSPerStream.count(streamIndex) > 0)
    {
      const auto ptsIncrease = packet->getPTS().value() - lastPTSPerStream.at(streamIndex);
      const auto dtsIncrease = packet->getDTS() - lastDTSPerStream.at(streamIndex);

      streamPacketCounters[streamIndex].packetCountPerPtsIncrease[ptsIncrease]++;
      streamPacketCounters[streamIndex].packetCountPerDtsIncrease[dtsIncrease]++;
    }

    lastPTSPerStream[streamIndex] = packet->getPTS().value();
    lastDTSPerStream[streamIndex] = packet->getDTS();

    streamPacketCounters[streamIndex].packetCount++;
    streamPacketCounters[streamIndex].packetCountPerDuration[packet->getDuration()]++;

    if (settings->showPackets)
    {
      auto ptsString = packet->getPTS() ? std::to_string(packet->getPTS().value()) : "No-PTS";
      std::cout << "  Packet " << packetIndex << ": StreamIndex " << packet->getStreamIndex()
                << " DTS " << packet->getDTS() << " PTS " << ptsString << " duration "
                << packet->getDuration() << " dataSize " << packet->getDataSize();
      const auto flags = to_string(packet->getFlags());
      if (!flags.empty())
        std::cout << " [" << to_string(packet->getFlags()) << "]";
      std::cout << "\n";
    }
    packetIndex++;
  }

  std::cout << "  Stream counts:\n";
  for (const auto [streamIndex, counters] : streamPacketCounters)
  {
    std::cout << "    Stream " << streamIndex << " : \n";
    std::cout << "      Packet count:            " << counters.packetCount << "\n";
    std::cout << "      Packet per duration:     " << to_string(counters.packetCountPerDuration)
              << "\n";
    std::cout << "      Packet per PTS increase: " << to_string(counters.packetCountPerPtsIncrease)
              << "\n";
    std::cout << "      Packet per DTS increase: " << to_string(counters.packetCountPerDtsIncrease)
              << "\n";
  }

  return 0;
}
