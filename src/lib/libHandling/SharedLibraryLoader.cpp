/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "SharedLibraryLoader.h"

#if (!defined(_WIN32) && !defined(_WIN64))
#include <limits.h>
#endif

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

bool SharedLibraryLoader::load(const std::filesystem::path &absolutePathOrLibName)
{
  this->unload();

#if (defined(_WIN32) || defined(_WIN64))
  this->libHandle = LoadLibraryA(absolutePathOrLibName.data());
#else
  this->libHandle = dlopen(absolutePathOrLibName.string().data(), RTLD_NOW | RTLD_LOCAL);
#endif

  if (this->libHandle == nullptr)
    return false;

  this->updateActuallyLoadedLibraryPath(absolutePathOrLibName.filename());

  return true;
};

void SharedLibraryLoader::updateActuallyLoadedLibraryPath(const std::filesystem::path &libraryName)
{
#if (defined(_WIN32) || defined(_WIN64))
  CHAR path[MAX_PATH] = {0};
  if (0 == GetModuleFileNameA(this->libHandle, path, MAX_PATH))
    return;
  const auto realPath = std::filesystem::path(path);
#else
  char path[PATH_MAX] = {0};
  const auto ret = dlinfo(this->libHandle, RTLD_DI_ORIGIN, &path);
  const auto realPath = std::filesystem::path(path) / libraryName;
#endif

  if (this->libraryPath.empty() && !realPath.empty())
    this->libraryPath = realPath;
}

} // namespace ffmpeg
