/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVFormat/wrappers/AVInputFormatWrapper.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVFormat/VersionToAVFormatTypes.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVInputFormat;
using ::testing::Return;

template <FFmpegVersion V> void runAVInputFormatWrapperTest()
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(getLibraryVerions(V)));

  AVInputFormatType<V> inputFormat;
  inputFormat.name      = "TestName";
  inputFormat.long_name = "TestNameLong";
  inputFormat.flags =
      (internal::avformat::AVFMT_GENERIC_INDEX + internal::avformat::AVFMT_NOBINSEARCH +
       internal::avformat::AVFMT_SEEK_TO_PTS);
  inputFormat.extensions = "TestExtension";
  inputFormat.mime_type  = "TestMimeType";

  AVInputFormatWrapper format(reinterpret_cast<AVInputFormat *>(&inputFormat), ffmpegLibraries);

  EXPECT_EQ(format.getName(), "TestName");
  EXPECT_EQ(format.getLongName(), "TestNameLong");

  const auto flags = format.getFlags();
  EXPECT_FALSE(flags.noFile);
  EXPECT_FALSE(flags.needNumber);
  EXPECT_FALSE(flags.experimental);
  EXPECT_FALSE(flags.showIDs);
  EXPECT_FALSE(flags.globalHeaders);
  EXPECT_FALSE(flags.noTiemstamps);
  EXPECT_TRUE(flags.genericIndex);
  EXPECT_FALSE(flags.tsDiscont);
  EXPECT_FALSE(flags.variableFPS);
  EXPECT_FALSE(flags.noDimensions);
  EXPECT_FALSE(flags.noStreams);
  EXPECT_TRUE(flags.noBinSearch);
  EXPECT_FALSE(flags.noGenSearch);
  EXPECT_FALSE(flags.noByteSeek);
  EXPECT_FALSE(flags.allowFlush);
  EXPECT_FALSE(flags.tsNonStrict);
  EXPECT_FALSE(flags.tsNegative);
  EXPECT_TRUE(flags.seekToPTS);

  EXPECT_EQ(format.getExtensions(), "TestExtension");
  EXPECT_EQ(format.getMimeType(), "TestMimeType");
}

} // namespace

class AVInputFormatWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST(AVInputFormatWrapperTest, shouldThrowIfLibraryNotSet)
{
  AVDummy                           dummyInputFormat;
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  EXPECT_THROW(
      AVInputFormatWrapper(reinterpret_cast<AVInputFormat *>(&dummyInputFormat), ffmpegLibraries),
      std::runtime_error);
}

TEST(AVInputFormatWrapperTest, shouldThrowIfAVInputFormatPointerIsNull)
{
  AVInputFormat *inputFormat{};
  auto           ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  EXPECT_THROW(AVInputFormatWrapper(inputFormat, ffmpegLibraries), std::runtime_error);
}

TEST_P(AVInputFormatWrapperTest, TestAVInputFormatWrapper)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVInputFormatWrapperTest);
}

INSTANTIATE_TEST_SUITE_P(AVFormatWrappers,
                         AVInputFormatWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avformat
