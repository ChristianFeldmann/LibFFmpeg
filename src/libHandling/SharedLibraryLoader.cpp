/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
