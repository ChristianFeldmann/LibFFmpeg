/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <iostream>

#include <libHandling/FFmpegLibrariesInterfaceBuilder.h>

int main(int argc, char const *argv[])
{
  const auto loadingResult = LibFFmpeg::FFmpegLibrariesInterfaceBuilder().tryLoadingOfLibraries();

  if (loadingResult.librariesInterface)
    std::cout << "Error when loading libraries: " << loadingResult.errorMessage << "\n";
  else
    std::cout << "Successfully loaded ffmpeg libraries.\n";

  std::cout << "\n ---- Logs -----\n";
  for (const auto &logLine : loadingResult.loadingLog)
    std::cout << logLine << "\n";

  return 0;
}
