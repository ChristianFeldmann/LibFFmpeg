/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVUtil/wrappers/AVFrameWrapper.h>
#include <AVUtil/wrappers/AVFrameWrapperInternal.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversionInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AVFrame;
using ffmpeg::internal::AVPixelFormat;
using internal::avutil::AVFrame_54;
using internal::avutil::AVFrame_55;
using internal::avutil::AVFrame_56;
using internal::avutil::AVFrame_57;
using internal::avutil::AVFrame_58;
using internal::avutil::AVPixFmtDescriptor_54;
using internal::avutil::AVPixFmtDescriptor_55;
using internal::avutil::AVPixFmtDescriptor_56;
using internal::avutil::AVPixFmtDescriptor_57;
using internal::avutil::AVPixFmtDescriptor_58;
using ::testing::Return;

template <typename AVFrameType> void runAVFrameWrapperTest(const LibraryVersions &version)
{
  constexpr auto TEST_PIXEL_FORMAT = AVPixelFormat(289);

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));
  ffmpegLibraries->functionChecks.avutilPixFmtDescGetExpectedFormat = TEST_PIXEL_FORMAT;

  int frameAllocCounter                  = 0;
  ffmpegLibraries->avutil.av_frame_alloc = [&frameAllocCounter]() {
    auto frame = new AVFrameType;
    frameAllocCounter++;
    return reinterpret_cast<AVFrame *>(frame);
  };

  int frameFreeCounter                  = 0;
  ffmpegLibraries->avutil.av_frame_free = [&frameFreeCounter](AVFrame **frame) {
    if (frame != nullptr && *frame != nullptr)
    {
      auto castFrame = reinterpret_cast<AVFrameType *>(*frame);
      delete (castFrame);
      *frame = nullptr;
      frameFreeCounter++;
    }
  };

  {
    AVFrameWrapper frame(ffmpegLibraries);
    EXPECT_EQ(frameAllocCounter, 1);
    EXPECT_EQ(frameFreeCounter, 0);

    {
      auto castFrame                 = reinterpret_cast<AVFrameType *>(frame.getFrame());
      castFrame->width               = 320;
      castFrame->height              = 160;
      castFrame->linesize[0]         = 123;
      castFrame->linesize[1]         = 223;
      castFrame->linesize[2]         = 323;
      castFrame->pts                 = 1000;
      castFrame->pict_type           = internal::AV_PICTURE_TYPE_P;
      castFrame->key_frame           = 1;
      castFrame->sample_aspect_ratio = {16, 9};
      castFrame->format              = TEST_PIXEL_FORMAT;
    }

    EXPECT_TRUE(frame);
    EXPECT_EQ(frame.getSize(), Size({320, 160}));
    EXPECT_EQ(frame.getLineSize(0), 123);
    EXPECT_EQ(frame.getLineSize(1), 223);
    EXPECT_EQ(frame.getLineSize(2), 323);
    EXPECT_EQ(frame.getPTS(), 1000);
    EXPECT_EQ(frame.getPictType(), PictureType::P);
    EXPECT_TRUE(frame.isKeyFrame());
    EXPECT_EQ(frame.getSampleAspectRatio(), Rational({16, 9}));
    EXPECT_EQ(frame.getPixelFormatDescriptor().name, "None");

    // Todo: Getting data untested so far.
  }

  EXPECT_EQ(ffmpegLibraries->functionCounters.avPixFmtDescGet, 1);
  EXPECT_EQ(frameAllocCounter, 1);
  EXPECT_EQ(frameFreeCounter, 1);
}

template <typename AVFrameType, typename AVPixFmtDescriptorType>
void runAVFrameWrapperTestDataAccess(const LibraryVersions &version)
{
  constexpr auto TEST_PIXEL_FORMAT = AVPixelFormat(289);

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));
  ffmpegLibraries->functionChecks.avutilPixFmtDescGetExpectedFormat = TEST_PIXEL_FORMAT;
}

} // namespace

class AVFrameWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_P(AVFrameWrapperTest, TestAVFrameWrapper)
{
  const auto version = GetParam();
  if (version.avutil.major == 54)
    runAVFrameWrapperTest<AVFrame_54>(version);
  else if (version.avutil.major == 55)
    runAVFrameWrapperTest<AVFrame_55>(version);
  else if (version.avutil.major == 56)
    runAVFrameWrapperTest<AVFrame_56>(version);
  else if (version.avutil.major == 57)
    runAVFrameWrapperTest<AVFrame_57>(version);
  else if (version.avutil.major == 58)
    runAVFrameWrapperTest<AVFrame_58>(version);
}

TEST_P(AVFrameWrapperTest, TestAVFrameWrapperDataAccess)
{
  const auto version = GetParam();
  if (version.avutil.major == 54)
    runAVFrameWrapperTestDataAccess<AVFrame_54, AVPixFmtDescriptor_54>(version);
  else if (version.avutil.major == 55)
    runAVFrameWrapperTestDataAccess<AVFrame_55, AVPixFmtDescriptor_55>(version);
  else if (version.avutil.major == 56)
    runAVFrameWrapperTestDataAccess<AVFrame_56, AVPixFmtDescriptor_56>(version);
  else if (version.avutil.major == 57)
    runAVFrameWrapperTestDataAccess<AVFrame_57, AVPixFmtDescriptor_57>(version);
  else if (version.avutil.major == 58)
    runAVFrameWrapperTestDataAccess<AVFrame_58, AVPixFmtDescriptor_58>(version);
}

INSTANTIATE_TEST_SUITE_P(AVUtilWrappers,
                         AVFrameWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avutil
