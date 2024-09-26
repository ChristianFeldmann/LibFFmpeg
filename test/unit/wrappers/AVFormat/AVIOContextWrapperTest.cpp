/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVFormat/wrappers/AVIOContextWrapper.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

namespace libffmpeg::avformat
{

namespace
{

using internal::AVIOContext;
using internal::functions::ReadPacketFunction;
using internal::functions::SeekFunction;
using internal::functions::WritePacketFunction;
using ::testing::Return;

class MockAVIOInputContext : public AVIOInputContext
{
public:
  MockAVIOInputContext(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
      : AVIOInputContext(ffmpegLibraries)
  {
  }

  std::optional<int> readData(uint8_t *buf, int buf_size) override { return {}; }
  std::optional<int> getFileSize() const override { return {}; }
  bool               seek(int64_t offset) override { return false; }
};

template <FFmpegVersion V> void runAVIOInputContextCreationTest()
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(getLibraryVerions(V)));

  int bufferAllocationCount          = 0;
  ffmpegLibraries->avutil.av_mallocz = [&bufferAllocationCount](size_t size)
  {
    ++bufferAllocationCount;
    auto buffer = new unsigned char[size];
    return reinterpret_cast<void *>(buffer);
  };

  int avioContextAllocateCounter = 0;
  ffmpegLibraries->avformat.avio_alloc_context =
      [&avioContextAllocateCounter](unsigned char       *buffer,
                                    int                  buffer_size,
                                    int                  write_flag,
                                    void                *opaque,
                                    ReadPacketFunction  *read_packet,
                                    WritePacketFunction *write_packet,
                                    SeekFunction        *seek)
  {
    ++avioContextAllocateCounter;
    EXPECT_GT(buffer_size, 0);
    EXPECT_NE(buffer, nullptr);
    EXPECT_EQ(write_flag, 0);
    EXPECT_NE(opaque, nullptr);
    EXPECT_NE(read_packet, nullptr);
    EXPECT_EQ(write_packet, nullptr);
    EXPECT_EQ(seek, nullptr);

    // Clean up buffer
    delete[] buffer;

    auto avioContext = new AVDummy;
    return reinterpret_cast<AVIOContext *>(avioContext);
  };

  int avioContextFreeCounter = 0;
  ffmpegLibraries->avformat.avio_context_free =
      [&avioContextFreeCounter](internal::AVIOContext **context)
  {
    ++avioContextFreeCounter;
    auto dummyCast = reinterpret_cast<AVDummy *>(*context);
    delete dummyCast;
  };

  int avFreePCounter               = 0;
  ffmpegLibraries->avutil.av_freep = [&avFreePCounter](void *buffer) { ++avFreePCounter; };

  {
    MockAVIOInputContext context(ffmpegLibraries);

    EXPECT_EQ(bufferAllocationCount, 1);
    EXPECT_EQ(avioContextAllocateCounter, 1);
    EXPECT_EQ(avioContextFreeCounter, 0);
    EXPECT_EQ(avioContextFreeCounter, 0);
  }

  EXPECT_EQ(bufferAllocationCount, 1);
  EXPECT_EQ(avioContextAllocateCounter, 1);

  if constexpr (V == FFmpegVersion::FFmpeg_2x)
  {
    EXPECT_EQ(avioContextFreeCounter, 0);
    EXPECT_EQ(avFreePCounter, 1);
  }
  else
  {
    EXPECT_EQ(avioContextFreeCounter, 1);
    EXPECT_EQ(avFreePCounter, 0);
  }
}

template <FFmpegVersion V> void runAVIOInputContextCreationBufferAllocationFails_ShouldNotThrow()
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(getLibraryVerions(V)));

  int bufferAllocationCount          = 0;
  ffmpegLibraries->avutil.av_mallocz = [&bufferAllocationCount](size_t size)
  {
    ++bufferAllocationCount;
    return nullptr;
  };

  MockAVIOInputContext context(ffmpegLibraries);
  EXPECT_EQ(bufferAllocationCount, 1);
  EXPECT_FALSE(context);
}

template <FFmpegVersion V> void runAVIOInputAllocContextFails_ShouldNotThrow()
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(getLibraryVerions(V)));

  int bufferAllocationCount          = 0;
  ffmpegLibraries->avutil.av_mallocz = [&bufferAllocationCount](size_t size)
  {
    ++bufferAllocationCount;
    auto buffer = new unsigned char[size];
    return reinterpret_cast<void *>(buffer);
  };

  int avioContextAllocateCounter = 0;
  ffmpegLibraries->avformat.avio_alloc_context =
      [&avioContextAllocateCounter](unsigned char       *buffer,
                                    int                  buffer_size,
                                    int                  write_flag,
                                    void                *opaque,
                                    ReadPacketFunction  *read_packet,
                                    WritePacketFunction *write_packet,
                                    SeekFunction        *seek)
  {
    ++avioContextAllocateCounter;

    // Clean up buffer
    delete[] buffer;

    return nullptr;
  };

  MockAVIOInputContext context(ffmpegLibraries);
  EXPECT_EQ(bufferAllocationCount, 1);
  EXPECT_EQ(avioContextAllocateCounter, 1);
  EXPECT_FALSE(context);
}

} // namespace

class AVIOInputContextTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST(AVIOInputContextTest, CreationOfAVIOInputContextWithInvalidFFmpegLibraries_shouldThrow)
{
  AVDummy                           dummyStream;
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  EXPECT_THROW(MockAVIOInputContext context(ffmpegLibraries), std::runtime_error);
}

TEST_P(AVIOInputContextTest,
       CreationOfAVIOInputContext_shouldCallMalloczAndAllocContextAndFreeContextAgain)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVIOInputContextCreationTest);
}

TEST_P(AVIOInputContextTest, CreationOfAVIOInputContext_notThrowIfAllocationOfBufferFails)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVIOInputContextCreationBufferAllocationFails_ShouldNotThrow);
}

TEST_P(AVIOInputContextTest, CreationOfAVIOInputContext_notThrowIfAVIOAllocContextFails)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVIOInputAllocContextFails_ShouldNotThrow);
}

INSTANTIATE_TEST_SUITE_P(AVIOContext,
                         AVIOInputContextTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace libffmpeg::avformat
