/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <common/InternalTypes.h>
#include <wrappers/AVFormat/AVCodecParametersWrapper.h>
#include <wrappers/AVFormat/AVCodecParametersWrapperInternal.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVCodecParameters;
using ffmpeg::internal::AVCOL_SPC_SMPTE240M;
using ffmpeg::internal::AVMEDIA_TYPE_AUDIO;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVPixFmtDescriptor;
using ffmpeg::internal::AVRational;
using internal::avformat::AVCodecParameters_56;
using internal::avformat::AVCodecParameters_57;
using internal::avformat::AVCodecParameters_58;
using internal::avformat::AVCodecParameters_59;
using internal::avformat::AVCodecParameters_60;
using ::testing::Return;

AVPixFmtDescriptor *getDescriptor(AVPixelFormat pix_fmt)
{
  EXPECT_EQ(pix_fmt, 479);
  return nullptr;
}

void runAVCodecParametersWrapperTestAVFormat56(const LibraryVersions &version)
{
  // Version 65 does not have a codecParameters struct yet.
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  AVCodecParameters_56     codecParameters;
  AVCodecParametersWrapper parameters(reinterpret_cast<AVCodecParameters *>(&codecParameters),
                                      ffmpegLibraries);

  EXPECT_EQ(parameters.getCodecType(), MediaType::Unknown);
  EXPECT_EQ(parameters.getCodecID(), ffmpeg::internal::AV_CODEC_ID_NONE);
  EXPECT_TRUE(parameters.getExtradata().empty());
  EXPECT_EQ(parameters.getSize(), Size());
  EXPECT_EQ(parameters.getColorspace(), ColorSpace::UNSPECIFIED);
  EXPECT_EQ(parameters.getPixelFormat().name, "Unknown");
  EXPECT_EQ(parameters.getSampleAspectRatio(), Rational());
}

template <typename AVCodecParametersType>
void runAVCodecParametersWrapperTest(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avutil.av_pix_fmt_desc_get = getDescriptor;

  std::array<uint8_t, 4> dummyExtradata     = {22, 56, 19, 22};
  std::vector<std::byte> referenceExtradata = {
      std::byte(22), std::byte(56), std::byte(19), std::byte(22)};

  AVCodecParametersType codecParameters;
  codecParameters.codec_type          = AVMEDIA_TYPE_AUDIO;
  codecParameters.codec_id            = static_cast<AVCodecID>(123);
  codecParameters.extradata           = dummyExtradata.data();
  codecParameters.extradata_size      = dummyExtradata.size();
  codecParameters.width               = 64;
  codecParameters.height              = 198;
  codecParameters.color_space         = AVCOL_SPC_SMPTE240M;
  codecParameters.format              = 479;
  codecParameters.sample_aspect_ratio = AVRational({23, 88});

  AVCodecParametersWrapper parameters(reinterpret_cast<AVCodecParameters *>(&codecParameters),
                                      ffmpegLibraries);

  EXPECT_EQ(parameters.getCodecType(), MediaType::Audio);
  EXPECT_EQ(parameters.getCodecID(), static_cast<AVCodecID>(123));
  EXPECT_EQ(parameters.getExtradata(), referenceExtradata);
  EXPECT_EQ(parameters.getSize(), Size({64, 198}));
  EXPECT_EQ(parameters.getColorspace(), ColorSpace::SMPTE240M);
  EXPECT_EQ(parameters.getPixelFormat().name, "None");
  EXPECT_EQ(parameters.getSampleAspectRatio(), Rational({23, 88}));
}

} // namespace

class AVCodecParametersWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST(AVCodecParametersWrapperTest, shouldThrowIfLibraryNotSet)
{
  AVCodecParameters_56              codecParameters;
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  EXPECT_THROW(AVCodecParametersWrapper(reinterpret_cast<AVCodecParameters *>(&codecParameters),
                                        ffmpegLibraries),
               std::runtime_error);
}

TEST(AVCodecParametersWrapperTest, shouldThrowIfAVInputFormatPointerIsNull)
{
  AVCodecParameters_56 *codecParameters{};
  auto                  ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  EXPECT_THROW(AVCodecParametersWrapper(reinterpret_cast<AVCodecParameters *>(codecParameters),
                                        ffmpegLibraries),
               std::runtime_error);
}

TEST_P(AVCodecParametersWrapperTest, TestAVInputFormatWrapper)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVCodecParametersWrapperTestAVFormat56(version);
  else if (version.avformat.major == 57)
    runAVCodecParametersWrapperTest<AVCodecParameters_57>(version);
  else if (version.avformat.major == 58)
    runAVCodecParametersWrapperTest<AVCodecParameters_58>(version);
  else if (version.avformat.major == 59)
    runAVCodecParametersWrapperTest<AVCodecParameters_59>(version);
  else if (version.avformat.major == 60)
    runAVCodecParametersWrapperTest<AVCodecParameters_60>(version);
}

INSTANTIATE_TEST_SUITE_P(AVFormatWrappers,
                         AVCodecParametersWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avformat
