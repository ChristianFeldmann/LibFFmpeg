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
#include <queue>
#include <sstream>

using namespace libffmpeg;
using PacketQueue = std::queue<avcodec::AVPacketWrapper>;

void showUsage()
{
  std::cout << "FilePacketComparer takes 2 input files and compares the packets of one\n";
  std::cout << "stream in each of the inputs. Packets are checked for data identity.\n";
  std::cout << "Usage:\n";
  std::cout
      << "  FilePacketComparer <filename1>:<stream_index1> <filename2>:<stream_index2> [options]\n";
  std::cout << "\n";
  std::cout << "Options:\n";
  std::cout << "  -loglevelDebug    Set log level to debug output\n";
  std::cout << "  -loglevelInfo     Set log level to info output\n";
  std::cout << "  -loglevelWarning  Set log level to warning output\n";
  std::cout << "  -libPath          Set a search path for the ffmpeg libraries\n";
}

struct CompareFile
{
  std::string filename;
  int         streamIndex{};
};

struct Settings
{
  CompareFile           file1{};
  CompareFile           file2{};
  bool                  showPackets{};
  std::filesystem::path libraryPath{};
  libffmpeg::LogLevel   logLevel{libffmpeg::LogLevel::Error};
};

std::optional<CompareFile> parseFileNameAndStreamIndex(const std::string &variable)
{
  const auto columnPos = variable.rfind(":");
  if (columnPos >= variable.size())
    return {};

  CompareFile file;

  file.filename = variable.substr(0, columnPos);
  if (!std::filesystem::exists(file.filename))
  {
    std::cout << "Given file " << file.filename << " not found.";
    return {};
  }

  try
  {
    file.streamIndex = std::stoi(variable.substr(columnPos + 1));
  }
  catch (const std::exception &e)
  {
    return {};
  }

  return file;
}

void compareQueuePacketsAndDrain(PacketQueue &queue1, PacketQueue &queue2)
{
  while (!queue1.empty() && !queue2.empty())
  {
    auto packet1 = std::move(queue1.front());
    auto packet2 = std::move(queue2.front());

    queue1.pop();
    queue2.pop();

    const auto streamIndex1 = packet1.getStreamIndex();
    const auto streamIndex2 = packet2.getStreamIndex();

    static int packetCount = -1;
    ++packetCount;

    if (packet1.getDuration() != packet2.getDuration())
    {
      std::cout << "Error comparing packet " << packetCount << ". Duration unequal ("
                << packet1.getDuration() << " vs " << packet2.getDuration() << ").\n";
      return;
    }

    // if (packet1.getDataSize() != packet2.getDataSize())
    //   std::cout << "Error comparing packet. Data size unequal (" << packet1.getDataSize() << " vs
    //   "
    //             << packet2.getDataSize() << ").\n";

    // COmpare data

    std::cout << "Match for packet " << packetCount << ". Duration " << packet1.getDuration()
              << ".\n";
  }
}

void logPacketQueueState(const std::string &queueName, const PacketQueue &queue)
{
  if (!queue.empty())
  {
    std::cout << "Queue " << queueName << " is not empty. " << queue.size()
              << " packets not matched.";
  }
}

std::optional<Settings> parseCommandLineArguments(int argc, char const *argv[])
{
  Settings settings;

  if (argc < 2)
  {
    std::cout << "Not enough arguemnts. Two files must be given";
    return {};
  }

  for (int i = 1; i < argc; ++i)
  {
    const auto argument = std::string(argv[i]);

    if (1 == i || 2 == i)
    {
      if (const auto file = parseFileNameAndStreamIndex(argument))
      {
        if (i == 1)
          settings.file1 = *file;
        else
          settings.file2 = *file;
      }
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

  Demuxer demuxer1(loadingResult.ffmpegLibraries);
  if (!demuxer1.openFile(settings->file1.filename))
  {
    std::cout << "Error when opening first input file : " + settings->file1.filename + "\n ";
    return 1;
  }
  std::cout << "Successfully opened file " + settings->file1.filename + ".\n";

  Demuxer demuxer2(loadingResult.ffmpegLibraries);
  if (!demuxer2.openFile(settings->file2.filename))
  {
    std::cout << "Error when opening second input file : " + settings->file2.filename + "\n ";
    return 1;
  }
  std::cout << "Successfully opened file " + settings->file2.filename + ".\n";

  const auto formatContext1 = demuxer1.getFormatContext();
  const auto inputFormat1   = formatContext1->getInputFormat();

  std::cout << "\nFile one info:\n";
  printInputFormat(formatContext1, inputFormat1);

  const auto formatContext2 = demuxer2.getFormatContext();
  const auto inputFormat2   = formatContext2->getInputFormat();

  std::cout << "\nFile two info:\n";
  printInputFormat(formatContext2, inputFormat2);

  PacketQueue packetQueue1;
  PacketQueue packetQueue2;

  // We don't assume that the packets from both files are iin the same order. So we have to queue
  // them for comparison.
  while (true)
  {
    auto packet1 = demuxer1.getNextPacket();
    if (packet1 && packet1->getStreamIndex() == settings->file1.streamIndex)
      packetQueue1.push(std::move(*packet1));

    auto packet2 = demuxer2.getNextPacket();
    if (packet2 && packet2->getStreamIndex() == settings->file2.streamIndex)
      packetQueue2.push(std::move(*packet2));

    compareQueuePacketsAndDrain(packetQueue1, packetQueue2);
    // We could possibly check if the queues run too full

    if (!packet1 && !packet2)
      break;
  }

  logPacketQueueState("1", packetQueue1);
  logPacketQueueState("2", packetQueue2);

  return 0;
}
