/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Demuxer.h>
#include <common/Formatting.h>

#include <cstddef>
#include <iostream>
#include <string>

using namespace libffmpeg;

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

void printInputFormat(const avformat::AVFormatContextWrapper *formatContext,
                      const avformat::AVInputFormatWrapper   &inputFormat)
{
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
}
