/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "LibrariesWithLogging.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace libffmpeg::test::integration
{

TEST(LibrariesLoading, LoadLibrariesAndLogVersion)
{
  auto libsAndLogs = LibrariesWithLogging();

  const auto librariesInfo = libsAndLogs.libraries->getLibrariesInfo();
  EXPECT_EQ(librariesInfo.size(), 4);

  for (const auto &libInfo : librariesInfo)
    std::cerr << "[          ] [INFO] Lib " << libInfo.name << " loaded from path " << libInfo.path
              << " with version " << libInfo.version << "\n";
}

TEST(LibrariesLoading, LoadLibrariesAndCheckLoggingCallback)
{
  auto libsAndLogs = LibrariesWithLogging();

  EXPECT_FALSE(libsAndLogs.logEntries.empty());
  EXPECT_TRUE(libsAndLogs.containsLogEntry({LogLevel::Info, "Logging function set successfully"}));
  EXPECT_TRUE(libsAndLogs.containsLogEntry(
      {LogLevel::Debug, "Successfully resolved function avutil_version"}));
  EXPECT_TRUE(libsAndLogs.containsLogEntry({LogLevel::Info, "Setting up av logging callback"}));
  EXPECT_TRUE(libsAndLogs.containsLogEntry(
      {LogLevel::Info,
       "Loading of ffmpeg libraries successfully finished. FFmpeg is ready to be used."}));

  EXPECT_FALSE(
      libsAndLogs.containsLogEntry({LogLevel::Info, "Disconnecting av logging callback"}));

  libsAndLogs.libraries.reset();

  EXPECT_TRUE(libsAndLogs.containsLogEntry({LogLevel::Info, "Disconnecting av logging callback"}));
}

} // namespace libffmpeg::test::integration
