/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <common/InternalTypes.h>
#include <wrappers/AVUtil/AVFrameWrapper.h>
#include <wrappers/AVUtil/AVFrameWrapperInternal.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AVFrame;
using internal::avutil::AVFrame_54;
using internal::avutil::AVFrame_55;
using internal::avutil::AVFrame_56;
using internal::avutil::AVFrame_57;
using internal::avutil::AVFrame_58;
using ::testing::Return;

template <typename T> AVFrame *avFrameTestAlloc()
{
  auto frame = new T;
  return reinterpret_cast<AVFrame *>(frame);
}

template <typename T> void avFrameFree(AVFrame **frame)
{
  if (frame != nullptr && *frame != nullptr)
  {
    auto castFrame = reinterpret_cast<T *>(*frame);
    delete (castFrame);
    *frame = nullptr;
  }
}

template <typename T> void setTestValuesInFrame(AVFrame *frame)
{
  auto castFrame                 = reinterpret_cast<T *>(frame);
  castFrame->width               = 320;
  castFrame->height              = 160;
  castFrame->linesize[0]         = 123;
  castFrame->linesize[1]         = 223;
  castFrame->linesize[2]         = 323;
  castFrame->pts                 = 1000;
  castFrame->pict_type           = internal::AV_PICTURE_TYPE_P;
  castFrame->key_frame           = 1;
  castFrame->sample_aspect_ratio = {16, 9};
}

template <typename AVFrameType> void runAVFrameWrapperTest(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avutil.av_frame_alloc = avFrameTestAlloc<AVFrameType>;
  ffmpegLibraries->avutil.av_frame_free  = avFrameFree<AVFrameType>;

  AVFrameWrapper frame(ffmpegLibraries);

  setTestValuesInFrame<AVFrameType>(frame.getFrame());

  EXPECT_TRUE(frame);
  EXPECT_EQ(frame.getSize(), Size({320, 160}));
  EXPECT_EQ(frame.getLineSize(0), 123);
  EXPECT_EQ(frame.getLineSize(1), 223);
  EXPECT_EQ(frame.getLineSize(2), 323);
  EXPECT_EQ(frame.getPTS(), 1000);
  EXPECT_EQ(frame.getPictType(), PictureType::P);
  EXPECT_TRUE(frame.isKeyFrame());
  EXPECT_EQ(frame.getSampleAspectRatio(), Rational({16, 9}));

  // Todo: Getting data untested so far.
  // Todo: PixelFormatDescriptor not tested
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

INSTANTIATE_TEST_SUITE_P(AVUtilWrappers,
                         AVFrameWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avutil
