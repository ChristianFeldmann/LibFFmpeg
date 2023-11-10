/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <libHandling/SharedLibraryLoader.h>

#include <gtest/gtest.h>

TEST(SharedLibraryLoader, DefaultValuesTest)
{
  ffmpeg::SharedLibraryLoader loader;

  EXPECT_FALSE(loader.isLoaded());
  EXPECT_TRUE(loader.getLibraryPath().empty());

  std::function<unsigned()> dummyFunctions;
  loader.tryResolveFunction(dummyFunctions, "NotResolvable");
  EXPECT_FALSE(dummyFunctions);

  EXPECT_FALSE(loader.load("libname", "/somePathThatDoesNotExist"));
  EXPECT_FALSE(loader.load("libname", "/tmp"));
  EXPECT_FALSE(loader.load("nonexistingLibName", {}));
}

TEST(SharedLibraryLoader, OpenDummyLibrary)
{
  ffmpeg::SharedLibraryLoader loader;

  const auto debugPath = std::filesystem::current_path();

  EXPECT_TRUE(loader.load("dummyLib", std::filesystem::current_path()));

  std::function<int()> getVersion;
  loader.tryResolveFunction(getVersion, "getVersion");

  EXPECT_TRUE(getVersion);
  EXPECT_EQ(getVersion(), 7263);
}
