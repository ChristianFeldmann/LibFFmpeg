/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVUtil/AVFrameSideDataWrapper.h>
#include <wrappers/AVUtil/AVFrameSideDataWrapperInternal.h>
#include <wrappers/AVUtil/AVMotionVectorConversionInternal.h>

#include <gtest/gtest.h>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AV_FRAME_DATA_DOWNMIX_INFO;
using ffmpeg::internal::AV_FRAME_DATA_MOTION_VECTORS;
using ffmpeg::internal::AVFrameSideData;
using ffmpeg::internal::avutil::AVMotionVector_54;
using ffmpeg::internal::avutil::AVMotionVector_55_56_57_58;
using internal::avutil::AVFrameSideData_54;
using internal::avutil::AVFrameSideData_55;
using internal::avutil::AVFrameSideData_56;
using internal::avutil::AVFrameSideData_57;
using internal::avutil::AVFrameSideData_58;
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

template <typename AVFrameSideDataType, typename MotionVectorType>
void runAVFrameWrapperTest(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  auto dummyRawMotionData = createDummyMotionData<MotionVectorType>();

  const auto &info     = typeid(dummyRawMotionData);
  const auto  typeSize = sizeof(MotionVectorType);

  AVFrameSideDataType rawSideData{};
  rawSideData.type = AV_FRAME_DATA_DOWNMIX_INFO;
  rawSideData.data = reinterpret_cast<uint8_t *>(dummyRawMotionData.data());
  rawSideData.size = 5 * sizeof(MotionVectorType);

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
public:
  static std::string
  getName(const testing::TestParamInfo<AVFrameSideDataWrapperTest::ParamType> &info);
};

TEST_P(AVFrameSideDataWrapperTest, TestGettingOfMotionVectors)
{
  const auto version = GetParam();
  switch (version.avutil.major)
  {
  case 54:
    runAVFrameWrapperTest<AVFrameSideData_54, AVMotionVector_54>(version);
    break;
  case 55:
    runAVFrameWrapperTest<AVFrameSideData_55, AVMotionVector_55_56_57_58>(version);
    break;
  case 56:
    runAVFrameWrapperTest<AVFrameSideData_56, AVMotionVector_55_56_57_58>(version);
    break;
  case 57:
    runAVFrameWrapperTest<AVFrameSideData_57, AVMotionVector_55_56_57_58>(version);
    break;
  case 58:
    runAVFrameWrapperTest<AVFrameSideData_58, AVMotionVector_55_56_57_58>(version);
    break;
  default:
    break;
  }
}

std::string AVFrameSideDataWrapperTest::getName(
    const testing::TestParamInfo<AVFrameSideDataWrapperTest::ParamType> &info)
{
  const auto version = info.param.ffmpegVersion;
  auto       name    = "FFmpeg_" + ffmpegVersionMapper.getName(version);
  std::replace(name.begin(), name.end(), '.', '_');
  return name;
}

INSTANTIATE_TEST_SUITE_P(AVUtilWrappers,
                         AVFrameSideDataWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         AVFrameSideDataWrapperTest::getName);

} // namespace ffmpeg::avutil