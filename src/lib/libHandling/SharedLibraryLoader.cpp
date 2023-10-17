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

  return this->isLoaded();
};

} // namespace ffmpeg
