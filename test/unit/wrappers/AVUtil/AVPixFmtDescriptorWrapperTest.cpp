/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <common/InternalTypes.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorWrapper.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorWrapperInternal.h>
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
void runAVPixFmtDescriptorWrapperTest(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avutil.av_pix_fmt_desc_get = getDescriptor<AVPixFmtDescriptorType>;

  const auto                pixelFormat = static_cast<AVPixelFormat>(123);
  AVPixFmtDescriptorWrapper wrapper(pixelFormat, ffmpegLibraries);

  EXPECT_EQ(wrapper.name, "Test");
  EXPECT_EQ(wrapper.numberOfComponents, 3);
  EXPECT_EQ(wrapper.shiftLumaToChroma.widthShift, 4);
  EXPECT_EQ(wrapper.shiftLumaToChroma.heightShift, 5);

  EXPECT_TRUE(wrapper.flags.bigEndian);
  EXPECT_FALSE(wrapper.flags.pallette);
  EXPECT_FALSE(wrapper.flags.bitwisePacked);
  EXPECT_FALSE(wrapper.flags.hwAccelerated);
  EXPECT_TRUE(wrapper.flags.planar);
  EXPECT_FALSE(wrapper.flags.rgb);
  EXPECT_FALSE(wrapper.flags.pseudoPallette);
  EXPECT_FALSE(wrapper.flags.hasAlphaPlane);
  EXPECT_FALSE(wrapper.flags.bayerPattern);
  EXPECT_FALSE(wrapper.flags.floatValues);

  EXPECT_EQ(wrapper.componentDescriptors.size(), 3);
  for (int i = 0; i < 3; ++i)
  {
    EXPECT_EQ(wrapper.componentDescriptors[i].plane, i);
    EXPECT_EQ(wrapper.componentDescriptors[i].step, 4);
    EXPECT_EQ(wrapper.componentDescriptors[i].offset, 3);
    EXPECT_EQ(wrapper.componentDescriptors[i].shift, 4);
    EXPECT_EQ(wrapper.componentDescriptors[i].depth, 6);
  }
}

} // namespace

class AVPixFmtDescriptorWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_P(AVPixFmtDescriptorWrapperTest, TestAVPixFmtDescriptorWrapper)
{
  const auto version = GetParam();
  if (version.avutil.major == 54)
    runAVPixFmtDescriptorWrapperTest<AVPixFmtDescriptor_54>(version);
  else if (version.avutil.major == 55)
    runAVPixFmtDescriptorWrapperTest<AVPixFmtDescriptor_55>(version);
  else if (version.avutil.major == 56)
    runAVPixFmtDescriptorWrapperTest<AVPixFmtDescriptor_56>(version);
  else if (version.avutil.major == 57)
    runAVPixFmtDescriptorWrapperTest<AVPixFmtDescriptor_57>(version);
  else if (version.avutil.major == 58)
    runAVPixFmtDescriptorWrapperTest<AVPixFmtDescriptor_58>(version);
}

INSTANTIATE_TEST_SUITE_P(AVUtilWrappers,
                         AVPixFmtDescriptorWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avutil
