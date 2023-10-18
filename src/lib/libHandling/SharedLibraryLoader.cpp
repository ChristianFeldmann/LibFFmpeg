/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "SharedLibraryLoader.h"

namespace ffmpeg
{

void SharedLibraryLoader::unload()
{
  if (this->isLoaded())
  {
#if (defined(_WIN32) || defined(_WIN64))
    FreeLibrary(this->libHandle);
#else
    dlclose(this->libHandle);
#endif
    this->libHandle = nullptr;
  }
  this->libraryPath.clear();
};

bool SharedLibraryLoader::load(const std::string_view absolutePathOrLibName)
{
  this->unload();

#if (defined(_WIN32) || defined(_WIN64))
  this->libHandle = LoadLibraryA(absolutePathOrLibName.data());
#else
  this->libHandle = dlopen(absolutePathOrLibName.data(), RTLD_NOW | RTLD_LOCAL);
#endif

  if (this->libHandle == nullptr)
    return false;

#if (defined(_WIN32) || defined(_WIN64))
  CHAR path[MAX_PATH] = {0};
  GetModuleFileNameA(this->libHandle, path, MAX_PATH);
  const auto realPath = std::string(path);
  if (this->libraryPath.empty() && !realPath.empty())
    this->libraryPath = realPath;
#else
    // Get path on linux
#endif

  return true;
};

} // namespace ffmpeg
