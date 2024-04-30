/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <libHandling/SharedLibraryLoader.h>

#include <gtest/gtest.h>

TEST(SharedLibraryLoader, DefaultValuesTest)
{
  libffmpeg::SharedLibraryLoader loader;

  EXPECT_FALSE(loader.isLoaded());
  EXPECT_TRUE(loader.getLibraryPath().empty());

  std::function<unsigned()> dummyFunctions;
  loader.tryResolveFunction(dummyFunctions, "NotResolvable");
  EXPECT_FALSE(dummyFunctions);

  EXPECT_FALSE(loader.load(std::filesystem::path("/nonexsitentPath")));
  EXPECT_FALSE(loader.load(std::filesystem::path("/tmp/nonexistentlibrary.so")));
  EXPECT_FALSE(loader.load(std::string("nonexistingLibName")));
}

TEST(SharedLibraryLoader, OpenDummyLibrary)
{
  libffmpeg::SharedLibraryLoader loader;

  const auto debugPath = std::filesystem::current_path();

#if (defined(_WIN32) || defined(_WIN64))
  const auto libraryName = "dummyLib.dll";
#elif (defined(__APPLE__))
  const auto libraryName = "libdummyLib.dylib";
#else
  const auto libraryName = "libdummyLib.so";
#endif
  ASSERT_TRUE(loader.load(std::filesystem::current_path() / libraryName));

  std::function<int()> getVersion;
  loader.tryResolveFunction(getVersion, "getVersion");

  ASSERT_TRUE(getVersion);
  EXPECT_EQ(getVersion(), 7263);
}
