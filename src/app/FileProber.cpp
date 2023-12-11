/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Demuxer.h>
#include <libHandling/FFmpegLibrariesBuilder.h>

#include <cstddef>
#include <filesystem>
#include <iomanip>
#include <iostream>

using namespace ffmpeg;

const auto FILE_NAME = std::string("testFile.webm");

std::string to_string(const avcodec::AVCodecDescriptorProperties &properties)
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

std::string to_string(const std::vector<std::string> &stringVec)
{
  if (stringVec.empty())
    return {};

  std::ostringstream stream;
  for (const auto &item : stringVec)
    stream << item << ", ";

  const auto outString = stream.str();
  return outString.substr(0, outString.length() - 2);
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

void printOutLog(const Log &log)
{
  std::cout << "\n ---- Log -----\n";
  for (const auto &logLine : log)
    std::cout << logLine << "\n";
}

struct Settings
{
  bool                  showPackets{};
  std::filesystem::path libraryPath{};
};

Settings parseCommandLineArguments(int argc, char const *argv[])
{
  Settings settings;
  for (int i = 1; i < argc; ++i)
  {
    const auto argument = std::string(argv[i]);
    if (argument == "-showAllPackets")
      settings.showPackets = true;
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
        std::cout << "Missing argument for parameter -libPath";
    }
  }
  return settings;
}

int main(int argc, char const *argv[])
{
  const auto settings = parseCommandLineArguments(argc, argv);

  const auto loadingResult = FFmpegLibrariesBuilder().tryLoadingOfLibraries();

  if (!loadingResult.ffmpegLibraries)
  {
    std::cout << "Error when loading libraries: " << loadingResult.errorMessage << "\n";
    printOutLog(loadingResult.loadingLog);
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
  const auto [openSuccessfull, openingLog] = demuxer.openFile(FILE_NAME);

  if (!openSuccessfull)
  {
    std::cout << "Error when opening input file : " + FILE_NAME + "\n ";
    printOutLog(openingLog);
    return 1;
  }
  else
    std::cout << "Successfully opened file " + FILE_NAME + ".\n";

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
    std::cout << "    Codec Type        : " << mediaTypeMapper.getName(stream.getCodecType())
              << "\n";

    std::cout << "    Codec Description\n";
    const auto codecDescriptor = stream.getCodecDescriptor();
    std::cout << "      Media Type      : "
              << mediaTypeMapper.getName(codecDescriptor.getMediaType()) << "\n";
    std::cout << "      Name            : " << codecDescriptor.getCodecName() << "\n";
    std::cout << "      Long Name       : " << codecDescriptor.getLongName() << "\n";
    std::cout << "      Properties      : " << to_string(codecDescriptor.getProperties()) << "\n";
    std::cout << "      Mime Types      : " << to_string(codecDescriptor.getMimeTypes()) << "\n";
    std::cout << "      Profiles        : " << to_string(codecDescriptor.getProfiles()) << "\n";

    std::cout << "    Average Framerate : " << to_string(stream.getAverageFrameRate()) << "\n";
    std::cout << "    Time Base         : " << to_string(stream.getTimeBase()) << "\n";
    std::cout << "    Frame Size        : " << to_string(stream.getFrameSize()) << "\n";
    std::cout << "    Colorspace        : " << colorSpaceMapper.getName(stream.getColorspace())
              << " - " << colorSpaceMapper.getText(stream.getColorspace()) << "\n";
    std::cout << "    Pixel Format      : " << stream.getPixelFormat().name << "\n";
    std::cout << "    Extradata         : " << to_string(stream.getExtradata()) << "\n";
  }

  std::map<int, int> streamPacketCounters;
  int                packetIndex = 0;
  while (auto packet = demuxer.getNextPacket())
  {
    const auto streamIndex = packet->getStreamIndex();
    streamPacketCounters[streamIndex]++;

    if (settings.showPackets)
    {
      std::cout << "  Packet " << packetIndex << ": StreamIndex " << packet->getStreamIndex()
                << " DTS " << packet->getDTS() << " PTS " << packet->getPTS() << " duration "
                << packet->getDuration() << " dataSize " << packet->getDataSize();
      const auto flags = to_string(packet->getFlags());
      if (!flags.empty())
        std::cout << " [" << to_string(packet->getFlags()) << "]";
      std::cout << "\n";
    }
    packetIndex++;
  }

  std::cout << "  Packet counts:\n";
  for (const auto [streamIndex, count] : streamPacketCounters)
    std::cout << "    Stream " << streamIndex << " : " << count << "\n";

  return 0;
}
