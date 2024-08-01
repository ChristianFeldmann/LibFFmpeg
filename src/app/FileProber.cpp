/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Demuxer.h>
#include <common/Formatting.h>
#include <libHandling/FFmpegLibrariesBuilder.h>

#include "Formatting.h"

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
  printInputFormat(formatContext, inputFormat);

  struct StreamCounters
  {
    int                    packetCount{0};
    std::map<int64_t, int> packetCountPerDuration{};
    std::map<int64_t, int> packetCountPerPtsIncrease{};
    std::map<int64_t, int> packetCountPerDtsIncrease{};
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
