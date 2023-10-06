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

bool SharedLibraryLoader::load(std::string absolutePathOrLibName)
{
  this->unload();

#if (defined(_WIN32) || defined(_WIN64))
  this->libHandle = LoadLibraryA(absolutePathOrLibName.c_str());
#else
  this->libHandle = dlopen(absolutePathOrLibName.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

  return this->isLoaded();
};

} // namespace LibFFmpeg
