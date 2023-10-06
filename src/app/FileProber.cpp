/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <iostream>

#include <Demuxer.h>
#include <libHandling/FFmpegLibrariesInterfaceBuilder.h>

const auto FILE_NAME = std::string("testFile.webm");

void printOutLog(const LibFFmpeg::Log &log)
{
  std::cout << "\n ---- Log -----\n";
  for (const auto &logLine : log)
    std::cout << logLine << "\n";
}

int main(int argc, char const *argv[])
{
  const auto loadingResult = LibFFmpeg::FFmpegLibrariesInterfaceBuilder().tryLoadingOfLibraries();

  if (!loadingResult.librariesInterface)
  {
    std::cout << "Error when loading libraries: " << loadingResult.errorMessage << "\n";
    printOutLog(loadingResult.loadingLog);
    return 1;
  }
  else
    std::cout << "Successfully loaded ffmpeg libraries.\n";

  LibFFmpeg::Demuxer demuxer(loadingResult.librariesInterface);
  const auto [openSuccessfull, openingLog] = demuxer.openFile(FILE_NAME);

  if (!openSuccessfull)
  {
    std::cout << "Error when opening input file : " + FILE_NAME + "\n ";
    printOutLog(openingLog);
    return 1;
  }
  else
    std::cout << "Successfully opened file " + FILE_NAME + ".\n";

  auto formatContext = demuxer.getFormatContext();
  auto inputFormat   = formatContext.getInputFormat();

  std::cout << "\nFile info:\n";

  std::cout << "  Format:\n";
  std::cout << "    Name " << inputFormat.getName() << "\n";

  return 0;
}
