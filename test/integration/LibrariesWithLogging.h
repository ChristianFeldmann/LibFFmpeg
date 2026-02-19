/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Demuxer.h>
#include <common/Formatting.h>
#include <libHandling/FFmpegLibrariesBuilder.h>

#include <gtest/gtest.h>

namespace libffmpeg::test::integration
{

// Generated with
// ffmpeg -y -f lavfi -i testsrc -f lavfi -i "sine=frequency=1000:duration=60" -f lavfi -i
// "sine=frequency=600:duration=60" -filter_complex amerge -t 1 -pix_fmt yuv420p
// TestFile_h264_aac_1s_320x240.mp4
constexpr auto TEST_FILE_NAME = "TestFile_h264_aac_1s_320x240.mp4";

class LibrariesWithLogging
{
public:
  LibrariesWithLogging()
  {
    const auto loadingResult =
        FFmpegLibrariesBuilder()
            .withAdditionalSearchPaths({".", ""})
            .withLoggingFunction([this](const LogLevel logLevel, const std::string &message)
                                 { this->loggingFunction(logLevel, message); },
                                 LogLevel::Debug)
            .tryLoadingOfLibraries();
    EXPECT_TRUE(loadingResult) << "Error loading libraries";
    this->libraries = loadingResult.ffmpegLibraries;
  }

  Demuxer openTestFileInDemuxer()
  {
    Demuxer demuxer(this->libraries);
    EXPECT_TRUE(demuxer.openFile(TEST_FILE_NAME))
        << "Opening test file " << TEST_FILE_NAME << " failed.";

    return demuxer;
  }

  void loggingFunction(const LogLevel logLevel, const std::string &message)
  {
    if (logLevel != LogLevel::Debug)
      std::cerr << "[          ] [" << to_string(logLevel) << "] " << message << "\n";
    this->logEntries.push_back({logLevel, message});
  }

  bool containsLogEntry(const LogEntry &logEntry)
  {
    return std::ranges::find(this->logEntries, logEntry) != this->logEntries.end();
  }

  std::vector<LogEntry>             logEntries{};
  std::shared_ptr<IFFmpegLibraries> libraries;
};

} // namespace libffmpeg::test::integration
