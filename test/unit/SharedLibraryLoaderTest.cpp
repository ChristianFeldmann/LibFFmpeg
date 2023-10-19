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

  EXPECT_FALSE(loader.load("/somePathThatDoesNotExist"));
  EXPECT_FALSE(loader.load("/tmp"));
  EXPECT_FALSE(loader.load(""));
}
