/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVUtil/wrappers/AVFrameSideDataWrapper.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVUtil/VersionToAVUtilTypes.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AV_FRAME_DATA_DOWNMIX_INFO;
using ffmpeg::internal::AV_FRAME_DATA_MOTION_VECTORS;
using ffmpeg::internal::AVFrameSideData;
using internal::avutil::AVMotionVector_54;
using internal::avutil::AVMotionVector_55_56_57_58;

using ::testing::Return;

template <typename MotionVectorType> std::array<MotionVectorType, 5> createDummyMotionData()
{
  static_assert(std::is_same_v<MotionVectorType, AVMotionVector_54> ||
                std::is_same_v<MotionVectorType, AVMotionVector_55_56_57_58>);

  std::array<MotionVectorType, 5> array;
  for (int i = 0; i < 5; ++i)
  {
    array[i].source = i;
    array[i].w      = i * 2;
    array[i].h      = i * 3;
    array[i].src_x  = i * 4;
    array[i].src_y  = i * 5;
    array[i].dst_x  = i * 6;
    array[i].dst_y  = i * 7;
    array[i].flags  = i * 8;
    if constexpr (std::is_same_v<MotionVectorType, AVMotionVector_55_56_57_58>)
    {
      array[i].motion_x     = i * 9;
      array[i].motion_y     = i * 10;
      array[i].motion_scale = i * 11;
    }
  }
  return array;
}

template <FFmpegVersion V> void runAVFrameSideDataWrapperTest()
{
  const auto version = getLibraryVerions(V);

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  auto dummyRawMotionData = createDummyMotionData<MotionVectorType<V>>();

  const auto &info     = typeid(dummyRawMotionData);
  const auto  typeSize = sizeof(MotionVectorType<V>);

  FrameSideDataType<V> rawSideData{};
  rawSideData.type = AV_FRAME_DATA_DOWNMIX_INFO;
  rawSideData.data = reinterpret_cast<uint8_t *>(dummyRawMotionData.data());
  rawSideData.size = 5 * typeSize;

  AVFrameSideDataWrapper sideData(reinterpret_cast<AVFrameSideData *>(&rawSideData),
                                  ffmpegLibraries);

  auto motionData = sideData.getMotionVectors();
  EXPECT_TRUE(motionData.empty());

  rawSideData.type = AV_FRAME_DATA_MOTION_VECTORS;
  motionData       = sideData.getMotionVectors();

  EXPECT_EQ(motionData.size(), 5);
  for (int i = 0; i < 5; ++i)
  {
    const auto &vector = motionData.at(i);
    EXPECT_EQ(vector.source, i);
    EXPECT_EQ(vector.w, i * 2);
    EXPECT_EQ(vector.h, i * 3);
    EXPECT_EQ(vector.src_x, i * 4);
    EXPECT_EQ(vector.src_y, i * 5);
    EXPECT_EQ(vector.dst_x, i * 6);
    EXPECT_EQ(vector.dst_y, i * 7);
    EXPECT_EQ(vector.flags, i * 8);

    if (version.avutil.major == 54)
    {
      EXPECT_EQ(vector.motion_x, 0);
      EXPECT_EQ(vector.motion_y, 0);
      EXPECT_EQ(vector.motion_scale, 0);
    }
    else
    {
      EXPECT_EQ(vector.motion_x, i * 9);
      EXPECT_EQ(vector.motion_y, i * 10);
      EXPECT_EQ(vector.motion_scale, i * 11);
    }
  }
}

} // namespace

class AVFrameSideDataWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_P(AVFrameSideDataWrapperTest, TestGettingOfMotionVectors)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVFrameSideDataWrapperTest);
}

INSTANTIATE_TEST_SUITE_P(AVUtilWrappers,
                         AVFrameSideDataWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avutil
