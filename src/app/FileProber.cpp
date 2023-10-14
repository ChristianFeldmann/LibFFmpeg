/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Demuxer.h>
#include <libHandling/FFmpegLibrariesInterfaceBuilder.h>

#include <cstddef>
#include <iomanip>
#include <iostream>

const auto FILE_NAME = std::string("testFile.webm");

std::string to_string(ffmpeg::avcodec::AVCodecDescriptorWrapper::Properties properties)
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

std::string to_string(const std::vector<std::string> stringVec)
{
  if (stringVec.empty())
    return {};

  std::ostringstream stream;
  for (const auto &item : stringVec)
    stream << item << ", ";

  const auto outString = stream.str();
  return outString.substr(0, outString.length() - 2);
}

std::string to_string(const ffmpeg::AVRational rational)
{
  return std::to_string(rational.num) + "/" + std::to_string(rational.den);
}

std::string to_string(const ffmpeg::Size size)
{
  return std::to_string(size.width) + "x" + std::to_string(size.height);
}

std::string to_string(const ffmpeg::ByteVector &bytes)
{
  std::ostringstream stream;
  for (const auto &byte : bytes)
    stream << "0x" << std::hex << std::to_integer<int>(byte) << " ";
  return stream.str();
}

void printOutLog(const ffmpeg::Log &log)
{
  std::cout << "\n ---- Log -----\n";
  for (const auto &logLine : log)
    std::cout << logLine << "\n";
}

int main(int argc, char const *argv[])
{
  const auto loadingResult = ffmpeg::FFmpegLibrariesInterfaceBuilder().tryLoadingOfLibraries();

  if (!loadingResult.librariesInterface)
  {
    std::cout << "Error when loading libraries: " << loadingResult.errorMessage << "\n";
    printOutLog(loadingResult.loadingLog);
    return 1;
  }
  else
    std::cout << "Successfully loaded ffmpeg libraries.\n";

  ffmpeg::Demuxer demuxer(loadingResult.librariesInterface);
  const auto [openSuccessfull, openingLog] = demuxer.openFile(FILE_NAME);

  if (!openSuccessfull)
  {
    std::cout << "Error when opening input file : " + FILE_NAME + "\n ";
    printOutLog(openingLog);
    return 1;
  }
  else
    std::cout << "Successfully opened file " + FILE_NAME + ".\n";

  auto       formatContext = demuxer.getFormatContext();
  const auto inputFormat   = formatContext.getInputFormat();

  std::cout << "\nFile info:\n";

  std::cout << "  Format:\n";
  std::cout << "    Name              : " << inputFormat.getName() << "\n";
  std::cout << "    Long Name         : " << inputFormat.getLongName() << "\n";
  std::cout << "    Flags             : " << to_string(inputFormat.getFlags()) << "\n";
  std::cout << "    Extensions        : " << inputFormat.getExtensions() << "\n";
  std::cout << "    Mime Type         : " << inputFormat.getMimeType() << "\n";
  std::cout << "    Start time        : " << formatContext.getStartTime() << "\n";
  std::cout << "    Duration          : " << formatContext.getDuration() << "\n";

  for (const auto &stream : formatContext.getStreams())
  {
    std::cout << "  Streams " << stream.getIndex() << ":\n";
    std::cout << "    Codec Type        : " << to_string(stream.getCodecType()) << "\n";
    std::cout << "    Codec Type Name   : " << stream.getCodecTypeName() << "\n";

    std::cout << "    Codec Description\n";
    const auto codecDescriptor = stream.getCodecDescriptor();
    std::cout << "      Media Type      : " << to_string(codecDescriptor.getMediaType()) << "\n";
    std::cout << "      Name            : " << codecDescriptor.getCodecName() << "\n";
    std::cout << "      Long Name       : " << codecDescriptor.getLongName() << "\n";
    std::cout << "      Properties      : " << to_string(codecDescriptor.getProperties()) << "\n";
    std::cout << "      Mime Types      : " << to_string(codecDescriptor.getMimeTypes()) << "\n";
    std::cout << "      Profiles        : " << to_string(codecDescriptor.getProfiles()) << "\n";

    std::cout << "    Average Framerate : " << to_string(stream.getAverageFrameRate()) << "\n";
    std::cout << "    Time Base         : " << to_string(stream.getTimeBase()) << "\n";
    std::cout << "    Frame Size        : " << to_string(stream.getFrameSize()) << "\n";
    std::cout << "    Colorspace        : " << to_string(stream.getColorspace()) << "\n";
    std::cout << "    Pixel Format      : " << to_string(stream.getPixelFormat()) << "\n";
    std::cout << "    Extradata         : " << to_string(stream.getExtradata()) << "\n";
  }

  return 0;
}
