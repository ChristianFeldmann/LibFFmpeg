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

namespace
{

std::string addLibraryExtensionIfNeeded(const std::string &libraryName)
{
  // On linux, the library will not be found if we don't add the extension
#if (defined(__linux__))
  if (libraryName.find(".so") == std::string::npos)
    return libraryName + ".so";
#elif (defined(__APPLE__))
  if (libraryName.size() <= 6 || libraryName.substr(libraryName.size() - 6) != ".dylib")
    return libraryName + ".dylib";
#endif
  return libraryName;
}

} // namespace

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

bool SharedLibraryLoader::load(const std::string &libraryName)
{
  this->unload();

  auto filenameToOpen = addLibraryExtensionIfNeeded(libraryName);

#if (defined(_WIN32) || defined(_WIN64))
  this->libHandle = LoadLibraryA(filenameToOpen.c_str());
#else
  this->libHandle = dlopen(filenameToOpen.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

  if (this->libHandle == nullptr)
    return false;

  this->updateActuallyLoadedLibraryPath(libraryName);
  return true;
};

bool SharedLibraryLoader::load(const std::filesystem::path &absolutePathToLibraryFile)
{
  this->unload();

  const auto fileStatus = std::filesystem::status(absolutePathToLibraryFile);
  if (fileStatus.type() == std::filesystem::file_type::not_found)
    return false;

#if (defined(_WIN32) || defined(_WIN64))
  this->libHandle = LoadLibraryA(absolutePathToLibraryFile.c_str());
#else
  this->libHandle = dlopen(absolutePathToLibraryFile.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

  if (this->libHandle == nullptr)
    return false;

  this->libraryPath = absolutePathToLibraryFile;
  return true;
}

void SharedLibraryLoader::updateActuallyLoadedLibraryPath(const std::string &libraryName)
{
#if (defined(_WIN32) || defined(_WIN64))
  CHAR path[MAX_PATH] = {0};
  if (0 == GetModuleFileNameA(this->libHandle, path, MAX_PATH))
    return;
  const auto realPath = std::filesystem::path(path);
#elif (defined(__APPLE__))
  // One of these may work. But I have to debug this on an actual mac.
  // for (auto i = 0; i < _dyld_image_count(); ++i)
  // {
  //   const auto imageName = _dyld_get_image_name(i);
  //   const auto probe_lib = jl_load_dynamic_library(image_name, JL_RTLD_DEFAULT);
  //   void *probe_handle = probe_lib->handle;
  //   uv_dlclose(probe_lib);

  //   // If the handle is the same as what was passed in (modulo mode bits), return this image name
  //   if (((intptr_t)handle & (-4)) == ((intptr_t)probe_handle & (-4)))
  //     return image_name;
  // }

  // if (handle)
  //   {
  //       Dl_info info;

  //       // Load binary path information from dladdr() API if available, else
  //       // use the user provided path
  //       if (dladdr(handle, &info) || !info.dli_fname)
  //       {
  //           return{ handle, libraryPath };
  //       }
  //       else
  //       {
  //           return{ handle, info.dli_fname };
  //       }
  //   }
  //   else
  //   {
  //       throw std::runtime_error{ "Cannot load shared library " + std::string{ libraryPath } };
  //   }
  std::filesystem::path realPath;
#else
  char       path[PATH_MAX] = {0};
  const auto ret            = dlinfo(this->libHandle, RTLD_DI_ORIGIN, &path);
  const auto realPath       = std::filesystem::path(path) / libraryName;
#endif

  if (this->libraryPath.empty() && !realPath.empty())
    this->libraryPath = realPath;
}

} // namespace ffmpeg
