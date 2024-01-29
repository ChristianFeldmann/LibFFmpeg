/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversionInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <wrappers/AVUtil/AVPixFmtDescriptorCreation.h>

#include <gtest/gtest.h>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVPixFmtDescriptor;
using internal::avutil::AVComponentDescriptor_54;
using internal::avutil::AVComponentDescriptor_55;
using internal::avutil::AVComponentDescriptor_57;
using internal::avutil::AVPixFmtDescriptor_54;
using internal::avutil::AVPixFmtDescriptor_55;
using internal::avutil::AVPixFmtDescriptor_56;
using internal::avutil::AVPixFmtDescriptor_57;
using internal::avutil::AVPixFmtDescriptor_58;
using ::testing::Return;

template <typename AVPixFmtDescriptorType>
void runAVPixFmtDescriptorConversionTest(const LibraryVersions &version)
{
  const auto TEST_PIXEL_FORMAT = static_cast<AVPixelFormat>(123);

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  PixelFormatDescriptor TEST_DESCRIPTOR;
  TEST_DESCRIPTOR.name                          = "Test";
  TEST_DESCRIPTOR.numberOfComponents            = 3;
  TEST_DESCRIPTOR.shiftLumaToChroma.widthShift  = 4;
  TEST_DESCRIPTOR.shiftLumaToChroma.heightShift = 5;
  TEST_DESCRIPTOR.flags.planar                  = true;
  TEST_DESCRIPTOR.flags.bigEndian               = true;
  TEST_DESCRIPTOR.componentDescriptors.push_back({0, 4, 3, 4, 6});
  TEST_DESCRIPTOR.componentDescriptors.push_back({1, 4, 3, 4, 6});
  TEST_DESCRIPTOR.componentDescriptors.push_back({2, 4, 3, 4, 6});

  auto rawPixelFormatDescriptor =
      createRawFormatDescriptor<AVPixFmtDescriptorType>(TEST_DESCRIPTOR);

  ffmpegLibraries->avutil.av_pix_fmt_desc_get = [&](AVPixelFormat pix_fmt) {
    EXPECT_EQ(pix_fmt, TEST_PIXEL_FORMAT);
    ++ffmpegLibraries->functionCounters.avPixFmtDescGet;
    return reinterpret_cast<const AVPixFmtDescriptor *>(&rawPixelFormatDescriptor);
  };

  const auto format = convertAVPixFmtDescriptor(TEST_PIXEL_FORMAT, ffmpegLibraries);

  EXPECT_EQ(format.name, "Test");
  EXPECT_EQ(format.numberOfComponents, 3);
  EXPECT_EQ(format.shiftLumaToChroma.widthShift, 4);
  EXPECT_EQ(format.shiftLumaToChroma.heightShift, 5);

  EXPECT_TRUE(format.flags.bigEndian);
  EXPECT_FALSE(format.flags.pallette);
  EXPECT_FALSE(format.flags.bitwisePacked);
  EXPECT_FALSE(format.flags.hwAccelerated);
  EXPECT_TRUE(format.flags.planar);
  EXPECT_FALSE(format.flags.rgb);
  EXPECT_FALSE(format.flags.pseudoPallette);
  EXPECT_FALSE(format.flags.hasAlphaPlane);
  EXPECT_FALSE(format.flags.bayerPattern);
  EXPECT_FALSE(format.flags.floatValues);

  EXPECT_EQ(format.componentDescriptors.size(), 3);
  for (int i = 0; i < 3; ++i)
  {
    EXPECT_EQ(format.componentDescriptors[i].plane, i);
    EXPECT_EQ(format.componentDescriptors[i].step, 4);
    EXPECT_EQ(format.componentDescriptors[i].offset, 3);
    EXPECT_EQ(format.componentDescriptors[i].shift, 4);
    EXPECT_EQ(format.componentDescriptors[i].depth, 6);
  }

  EXPECT_EQ(format, TEST_DESCRIPTOR);
}

} // namespace

class AVPixFmtDescriptorConversionTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST(AVPixFmtDescriptorConversionTest, TestDefaultConstructor)
{
  PixelFormatDescriptor format;

  EXPECT_EQ(format.name, "Unknown");
  EXPECT_EQ(format.numberOfComponents, 0);
  EXPECT_EQ(format.shiftLumaToChroma.widthShift, 0);
  EXPECT_EQ(format.shiftLumaToChroma.heightShift, 0);

  EXPECT_FALSE(format.flags.bigEndian);
  EXPECT_FALSE(format.flags.pallette);
  EXPECT_FALSE(format.flags.bitwisePacked);
  EXPECT_FALSE(format.flags.hwAccelerated);
  EXPECT_FALSE(format.flags.planar);
  EXPECT_FALSE(format.flags.rgb);
  EXPECT_FALSE(format.flags.pseudoPallette);
  EXPECT_FALSE(format.flags.hasAlphaPlane);
  EXPECT_FALSE(format.flags.bayerPattern);
  EXPECT_FALSE(format.flags.floatValues);

  EXPECT_EQ(format.componentDescriptors.size(), 0);
}

TEST_P(AVPixFmtDescriptorConversionTest, TestAVPixFmtDescriptorConversion)
{
  const auto version = GetParam();
  if (version.avutil.major == 54)
    runAVPixFmtDescriptorConversionTest<AVPixFmtDescriptor_54>(version);
  else if (version.avutil.major == 55)
    runAVPixFmtDescriptorConversionTest<AVPixFmtDescriptor_55>(version);
  else if (version.avutil.major == 56)
    runAVPixFmtDescriptorConversionTest<AVPixFmtDescriptor_56>(version);
  else if (version.avutil.major == 57)
    runAVPixFmtDescriptorConversionTest<AVPixFmtDescriptor_57>(version);
  else if (version.avutil.major == 58)
    runAVPixFmtDescriptorConversionTest<AVPixFmtDescriptor_58>(version);
}

INSTANTIATE_TEST_SUITE_P(AVUtilWrappers,
                         AVPixFmtDescriptorConversionTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avutil
