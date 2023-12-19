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

template <typename AVComponentDescriptorType>
AVComponentDescriptorType getComponentDescriptor(int index)
{
  AVComponentDescriptorType componentDescriptor;
  componentDescriptor.plane = index;
  componentDescriptor.shift = 4;

  if constexpr (std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_54> ||
                std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_55>)
  {
    componentDescriptor.step_minus1  = 3;
    componentDescriptor.offset_plus1 = 4;
    componentDescriptor.depth_minus1 = 5;
  }
  if constexpr (std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_55> ||
                std::is_same_v<AVComponentDescriptorType, AVComponentDescriptor_57>)
  {
    componentDescriptor.step   = 4;
    componentDescriptor.offset = 3;
    componentDescriptor.depth  = 6;
  }

  return componentDescriptor;
}

template <typename T> AVPixFmtDescriptor *getDescriptor(AVPixelFormat pix_fmt)
{
  if constexpr (std::is_same_v<T, AVPixFmtDescriptor_54>)
  {
    static AVPixFmtDescriptor_54 descriptor;
    descriptor.name          = "Test";
    descriptor.nb_components = 3;
    descriptor.log2_chroma_w = 4;
    descriptor.log2_chroma_h = 5;
    descriptor.flags         = 0b10001; // Planar and bigEndia flag
    descriptor.comp[0]       = getComponentDescriptor<AVComponentDescriptor_54>(0);
    descriptor.comp[1]       = getComponentDescriptor<AVComponentDescriptor_54>(1);
    descriptor.comp[2]       = getComponentDescriptor<AVComponentDescriptor_54>(2);
    return reinterpret_cast<AVPixFmtDescriptor *>(&descriptor);
  }
  if constexpr (std::is_same_v<T, AVPixFmtDescriptor_55> ||
                std::is_same_v<T, AVPixFmtDescriptor_56>)
  {
    static AVPixFmtDescriptor_55 descriptor;
    descriptor.name          = "Test";
    descriptor.nb_components = 3;
    descriptor.log2_chroma_w = 4;
    descriptor.log2_chroma_h = 5;
    descriptor.flags         = 0b10001; // Planar and bigEndia flag
    descriptor.comp[0]       = getComponentDescriptor<AVComponentDescriptor_55>(0);
    descriptor.comp[1]       = getComponentDescriptor<AVComponentDescriptor_55>(1);
    descriptor.comp[2]       = getComponentDescriptor<AVComponentDescriptor_55>(2);
    return reinterpret_cast<AVPixFmtDescriptor *>(&descriptor);
  }
  if constexpr (std::is_same_v<T, AVPixFmtDescriptor_57> ||
                std::is_same_v<T, AVPixFmtDescriptor_58>)
  {
    static AVPixFmtDescriptor_57 descriptor;
    descriptor.name          = "Test";
    descriptor.nb_components = 3;
    descriptor.log2_chroma_w = 4;
    descriptor.log2_chroma_h = 5;
    descriptor.flags         = 0b10001; // Planar and bigEndia flag
    descriptor.comp[0]       = getComponentDescriptor<AVComponentDescriptor_57>(0);
    descriptor.comp[1]       = getComponentDescriptor<AVComponentDescriptor_57>(1);
    descriptor.comp[2]       = getComponentDescriptor<AVComponentDescriptor_57>(2);
    return reinterpret_cast<AVPixFmtDescriptor *>(&descriptor);
  }

  return nullptr;
}

template <typename AVPixFmtDescriptorType>
void runAVPixFmtDescriptorConversionTest(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avutil.av_pix_fmt_desc_get = getDescriptor<AVPixFmtDescriptorType>;

  const auto pixelFormat = static_cast<AVPixelFormat>(123);
  const auto format      = convertAVPixFmtDescriptor(pixelFormat, ffmpegLibraries);

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
