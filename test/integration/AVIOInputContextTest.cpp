/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "LibrariesWithLogging.h"

#include <AVFormat/wrappers/AVIOContextWrapper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>

namespace libffmpeg::test::integration
{

namespace
{

class FileReaderIOContext : public avformat::AVIOInputContext
{
public:
  FileReaderIOContext(const Path &path, std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
      : avformat::AVIOInputContext(ffmpegLibraries), filePath(path)
  {
    this->inputFile.open(path, std::ios::binary);
    if (!this->inputFile.is_open())
      throw std::runtime_error("Error opening file " + path.string());
  }

  std::optional<int> readData(uint8_t *buf, int buf_size) override
  {
    this->inputFile.read(reinterpret_cast<char *>(buf), static_cast<std::streamsize>(buf_size));
    const auto actualRead = this->inputFile.gcount();
    if (actualRead > 0)
      return static_cast<int>(actualRead);
    return {};
  }

  std::optional<int> getFileSize() const override
  {
    return std::filesystem::file_size(this->filePath);
  }

  bool seek(int64_t offset) override
  {
    if (this->inputFile.fail() && this->inputFile.eof())
      this->inputFile.clear();

    if (this->inputFile.fail())
      return false;

    this->inputFile.seekg(offset, std::ios_base::beg);
    return !this->inputFile.fail();
  }

private:
  Path          filePath{};
  std::ifstream inputFile{};
};

} // namespace

TEST(AVIOInputContext, DemuxAndDecodeUsingAVIOContext_shouldDemuxAndDecodeAllFramesCorrectly)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto context = std::make_unique<FileReaderIOContext>(TEST_FILE_NAME, libsAndLogs.libraries);

  Demuxer demuxer(libsAndLogs.libraries);
  demuxer.openInput(std::move(context));
  const auto formatContext = demuxer.getFormatContext();

  EXPECT_EQ(formatContext->getDuration(), 1000000);
  EXPECT_EQ(formatContext->getNumberStreams(), 2);

  std::array<int, 2> packetCounters = {0, 0};
  while (auto packet = demuxer.getNextPacket())
  {
    const auto streamIndex = packet->getStreamIndex();
    EXPECT_TRUE(streamIndex == 0 || streamIndex == 1);

    packetCounters[streamIndex]++;
  }

  EXPECT_EQ(45, packetCounters.at(0));
  EXPECT_EQ(25, packetCounters.at(1));
}

} // namespace libffmpeg::test::integration
