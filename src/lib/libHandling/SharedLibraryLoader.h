/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <filesystem>
#include <functional>

#if (defined(_WIN32) || defined(_WIN64))
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#else
#include <dlfcn.h>
#endif

namespace LibFFmpeg
{

class SharedLibraryLoader
{
public:
  SharedLibraryLoader() = default;

  void unload();
  bool load(std::filesystem::path pathToLib);

  auto getLibraryPath() const { return this->libraryPath; }

  bool isLoaded() const { return this->libHandle != nullptr; }
       operator bool() const { return this->isLoaded(); }

  template <typename T>
  void tryResolveFunction(std::function<T> &function, const std::string &functionName) const
  {
    if (!this->isLoaded() || functionName.empty())
      return;

#if (defined(_WIN32) || defined(_WIN64))
    const auto addr = GetProcAddress(this->libHandle, functionName.c_str());
#else
    const auto addr = dlsym(this->libHandle, functionName.c_str());
#endif

    if (addr != nullptr)
      function = reinterpret_cast<T *>(addr);
  }

private:
#if (defined(_WIN32) || defined(_WIN64))
  HINSTANCE libHandle{nullptr};
#else
  void *libHandle{};
#endif

  std::filesystem::path libraryPath;
};

} // namespace LibFFmpeg
