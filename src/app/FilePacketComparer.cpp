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
using DataQueue   = std::deque<std::byte>;

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

/* For some codecs (e.g. aac) the packet sizes and durations must be identical.
 * For other codecs (e.g. raw data like pcm), the size of the packets is not so
 * important but the data (byte by byte) must match.
 */
enum class ComparisonMode
{
  Packets,
  Data
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

Demuxer openInput(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries, const std::string &filename)
{
  Demuxer demuxer(ffmpegLibraries);
  if (!demuxer.openFile(filename))
    throw std::runtime_error("Error when opening first input file : " + filename + "\n ");

  std::cout << "Successfully opened file " + filename + ".\n";

  const auto formatContext = demuxer.getFormatContext();
  const auto inputFormat   = formatContext->getInputFormat();

  std::cout << "\nFile info:\n";
  printInputFormat(formatContext, inputFormat);
  std::cout << "\n";

  return demuxer;
}

ComparisonMode checkStreamsAndGetComparMode(Demuxer & demuxer1,
                                            Demuxer & demuxer2,
                                            const int streamIndex1,
                                            const int streamIndex2)
{
  const auto streams1 = demuxer1.getFormatContext()->getStreams();
  const auto streams2 = demuxer2.getFormatContext()->getStreams();

  if (streamIndex1 >= streams1.size())
    throw std::runtime_error("Given stream index for stream 1 not found.");
  if (streamIndex2 >= streams2.size())
    throw std::runtime_error("Given stream index for stream 2 not found.");

  const auto codecType1 = streams1.at(streamIndex1).getCodecType();
  const auto codecType2 = streams2.at(streamIndex2).getCodecType();
  if (codecType1 != codecType2)
    throw std::runtime_error("Stream types of streams to compare do not match. Type 1 " +
                             avutil::mediaTypeMapper.getName(codecType1) + " and type 2 " +
                             avutil::mediaTypeMapper.getName(codecType2));

  const auto codecDescriptor1 = streams1.at(streamIndex1).getCodecDescriptor();
  if (!codecDescriptor1)
    throw std::runtime_error("Error getting codec descriptor for stream 1.");
  const auto codecDescriptor2 = streams2.at(streamIndex2).getCodecDescriptor();
  if (!codecDescriptor2)
    throw std::runtime_error("Error getting codec descriptor for stream 2.");

  if (codecDescriptor1->codecName != codecDescriptor2->codecName)
    throw std::runtime_error("Codec name of streams to compare do not match. Name 1 " +
                             codecDescriptor1->codecName + " and name 2 " +
                             codecDescriptor2->codecName);

  if (codecDescriptor1->codecName.find("pcm") == 0)
    return ComparisonMode::Data;
  return ComparisonMode::Packets;
}

bool compareData(const ByteVector &data1, const ByteVector &data2)
{
  return std::equal(data1.begin(), data1.end(), data2.begin(), data2.end());
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

    if (packet1.getDataSize() != packet2.getDataSize())
    {
      std::cout << "Error comparing packet " << packetCount << ". Data size unequal ("
                << packet1.getDataSize() << " vs " << packet2.getDataSize() << ").\n";
      return;
    }

    if (!compareData(packet1.getData(), packet2.getData()))
    {
      std::cout << "Error comparing packet " << packetCount << ". Data unequal.\n";
      return;
    }

    std::cout << "Match for packet " << packetCount << ". Duration " << packet1.getDuration()
              << ".\n";
  }
}

void compareDataQueueAndDrain(DataQueue &queue1, DataQueue &queue2)
{
  int  bytesMatched = 0;
  bool matched      = true;

  while (!queue1.empty() && !queue2.empty())
  {
    const auto byte1 = queue1.front();
    const auto byte2 = queue2.front();

    queue1.pop_front();
    queue2.pop_front();

    static int byteCounter = -1;
    ++byteCounter;
    ++bytesMatched;

    if (byte1 != byte2)
    {
      std::cout << "Error comparing data. Byte " << byteCounter << " differs.\n";
      matched = false;
    }
  }

  if (bytesMatched > 0)
    std::cout << (matched ? "Successfully matched " : "Failed to match ") << bytesMatched
              << " bytes.\n";
}

template <typename T> void logQueueState(const std::string &queueName, const T &queue)
{
  if (!queue.empty())
  {
    std::cout << "Queue " << queueName << " is not empty. " << queue.size()
              << " items not matched.";
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

  auto demuxer1 = openInput(loadingResult.ffmpegLibraries, settings->file1.filename);
  auto demuxer2 = openInput(loadingResult.ffmpegLibraries, settings->file2.filename);

  const auto compareMode = checkStreamsAndGetComparMode(
      demuxer1, demuxer2, settings->file1.streamIndex, settings->file2.streamIndex);

  PacketQueue packetQueue[2];
  DataQueue   dataQueue[2];

  auto addPacketToQueue = [&packetQueue, &dataQueue, compareMode](avcodec::AVPacketWrapper &&packet,
                                                                  const int streamIndex,
                                                                  const int queueIndex) {
    const auto packetStreamIndex = packet.getStreamIndex();
    if (packetStreamIndex != streamIndex)
      return;
    if (compareMode == ComparisonMode::Packets)
      packetQueue[queueIndex].push(std::move(packet));
    else
    {
      const auto data = packet.getData();
      dataQueue[queueIndex].insert(dataQueue[queueIndex].end(), data.begin(), data.end());
    }
  };

  while (true)
  {
    auto packet1 = demuxer1.getNextPacket();
    auto packet2 = demuxer2.getNextPacket();

    if (!packet1 && !packet2)
      break;

    if (packet1)
      addPacketToQueue(std::move(*packet1), settings->file1.streamIndex, 0);
    if (packet2)
      addPacketToQueue(std::move(*packet2), settings->file2.streamIndex, 1);

    if (compareMode == ComparisonMode::Packets)
      compareQueuePacketsAndDrain(packetQueue[0], packetQueue[1]);
    else
      compareDataQueueAndDrain(dataQueue[0], dataQueue[1]);
  }

  if (compareMode == ComparisonMode::Packets)
  {
    logQueueState("1", packetQueue[0]);
    logQueueState("2", packetQueue[1]);
  }
  else
  {
    logQueueState("1", dataQueue[0]);
    logQueueState("2", dataQueue[1]);
  }

  return 0;
}
