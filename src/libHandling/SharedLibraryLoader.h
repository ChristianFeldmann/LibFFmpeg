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

#pragma once

#include <QLibrary>
#include <filesystem>

typedef void (*FunctionPointer)();

namespace LibFFmpeg
{

class SharedLibraryLoader
{
public:
  SharedLibraryLoader() = default;

  void            unload();
  bool            load(std::filesystem::path pathToLib);
  FunctionPointer resolve(std::string functionName);

  auto getLibraryPath() const { return this->libraryPath; }

  operator bool() const { return this->library.isLoaded(); }

  template <typename T>
  void tryResolveFunction(std::function<T> &function, const char *symbolName) const
  {
    if (auto ptr = this->library.resolve(symbolName))
      function = reinterpret_cast<T *>(ptr);
  }

private:
  // Todo: Replace this with alternative
  QLibrary library;

  std::filesystem::path libraryPath;
};

} // namespace LibFFmpeg
