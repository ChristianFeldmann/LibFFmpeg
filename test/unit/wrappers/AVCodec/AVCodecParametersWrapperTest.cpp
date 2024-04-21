/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecParametersWrapper.h>
#include <AVCodec/wrappers/AVCodecParametersWrapperInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include "VersionToAVCodecTypes.h"

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avcodec
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
using ::testing::Return;

void runAVCodecParametersWrapperTestAVFormat56(const LibraryVersions &version)
{
  // Version 56 does not have a codecParameters struct yet.
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpeg::internal::avcodec::AVCodecParameters_56 codecParameters;
  AVCodecParametersWrapper parameters(reinterpret_cast<AVCodecParameters *>(&codecParameters),
                                      ffmpegLibraries);

  EXPECT_EQ(parameters.getCodecType(), avutil::MediaType::Unknown);
  EXPECT_EQ(parameters.getCodecID(), ffmpeg::internal::AV_CODEC_ID_NONE);
  EXPECT_TRUE(parameters.getExtradata().empty());
  EXPECT_EQ(parameters.getSize(), Size());
  EXPECT_EQ(parameters.getColorspace(), avutil::ColorSpace::UNSPECIFIED);
  EXPECT_EQ(parameters.getPixelFormat().name, "Unknown");
  EXPECT_EQ(parameters.getSampleAspectRatio(), Rational());
}

template <FFmpegVersion V> void runAVCodecParametersWrapperTest()
{
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    // No codec parameters in V2 (AVFormat 56)
    return;

  const auto version           = getLibraryVerions(V);
  const auto TEST_PIXEL_FORMAT = static_cast<AVPixelFormat>(637);

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->functionChecks.avutilPixFmtDescGetExpectedFormat = TEST_PIXEL_FORMAT;

  std::array<uint8_t, 4> TEST_EXTRADATA = {22, 56, 19, 22};

  AVCodecParametersType<V> codecParameters;
  codecParameters.codec_type          = AVMEDIA_TYPE_AUDIO;
  codecParameters.codec_id            = static_cast<AVCodecID>(123);
  codecParameters.extradata           = TEST_EXTRADATA.data();
  codecParameters.extradata_size      = static_cast<int>(TEST_EXTRADATA.size());
  codecParameters.width               = 64;
  codecParameters.height              = 198;
  codecParameters.color_space         = AVCOL_SPC_SMPTE240M;
  codecParameters.format              = TEST_PIXEL_FORMAT;
  codecParameters.sample_aspect_ratio = AVRational({23, 88});

  AVCodecParametersWrapper parameters(reinterpret_cast<AVCodecParameters *>(&codecParameters),
                                      ffmpegLibraries);

  EXPECT_EQ(parameters.getCodecType(), avutil::MediaType::Audio);
  EXPECT_EQ(parameters.getCodecID(), static_cast<AVCodecID>(123));
  EXPECT_EQ(parameters.getExtradata(), dataArrayToByteVector(TEST_EXTRADATA));
  EXPECT_EQ(parameters.getSize(), Size({64, 198}));
  EXPECT_EQ(parameters.getColorspace(), avutil::ColorSpace::SMPTE240M);
  EXPECT_EQ(parameters.getPixelFormat().name, "None");
  EXPECT_EQ(parameters.getSampleAspectRatio(), Rational({23, 88}));

  EXPECT_EQ(ffmpegLibraries->functionCounters.avPixFmtDescGet, 1);
}

} // namespace

class AVCodecParametersWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST(AVCodecParametersWrapperTest, shouldThrowIfLibraryNotSet)
{
  AVDummy                           dummyCodecParameters;
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  EXPECT_THROW(AVCodecParametersWrapper(
                   reinterpret_cast<AVCodecParameters *>(&dummyCodecParameters), ffmpegLibraries),
               std::runtime_error);
}

TEST(AVCodecParametersWrapperTest, shouldThrowIfAVInputFormatPointerIsNull)
{
  AVCodecParameters *codecParameters{};
  auto               ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  EXPECT_THROW(AVCodecParametersWrapper(codecParameters, ffmpegLibraries), std::runtime_error);
}

TEST_P(AVCodecParametersWrapperTest, TestAVInputFormatWrapper)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVCodecParametersWrapperTestAVFormat56(version);
  else
    RUN_TEST_FOR_VERSION(version, runAVCodecParametersWrapperTest);
}

INSTANTIATE_TEST_SUITE_P(AVFormatWrappers,
                         AVCodecParametersWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avcodec
