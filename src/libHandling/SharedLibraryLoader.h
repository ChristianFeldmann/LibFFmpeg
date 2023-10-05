/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
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
