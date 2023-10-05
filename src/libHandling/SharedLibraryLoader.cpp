/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "SharedLibraryLoader.h"

namespace LibFFmpeg
{

void SharedLibraryLoader::unload()
{
  this->library.unload();
  this->libraryPath.clear();
};

bool SharedLibraryLoader::load(std::filesystem::path pathToLib)
{
  const auto filePathQString = QString::fromStdString(pathToLib.string());
  this->library.setFileName(filePathQString);
  const auto success = this->library.load();
  if (success)
    this->libraryPath = pathToLib;
  return success;
};

FunctionPointer SharedLibraryLoader::resolve(std::string functionName)
{
  auto functionPointer = this->library.resolve(functionName.c_str());
  return functionPointer;
};

} // namespace LibFFmpeg
