/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecContextWrapperInternal.h>
#include <AVFormat/wrappers/AVCodecParametersWrapperInternal.h>
#include <AVFormat/wrappers/AVStreamWrapper.h>
#include <AVFormat/wrappers/AVStreamWrapperInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVCodecContext;
using ffmpeg::internal::AVCodecDescriptor;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVCodecParameters;
using ffmpeg::internal::AVCOL_SPC_FCC;
using ffmpeg::internal::AVMEDIA_TYPE_AUDIO;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;
using ffmpeg::internal::AVStream;
using internal::avcodec::AVCodecContext_56;
using internal::avcodec::AVCodecContext_57;
using internal::avcodec::AVCodecContext_58;
using internal::avcodec::AVCodecContext_59;
using internal::avcodec::AVCodecContext_60;
using internal::avformat::AVCodecParameters_57;
using internal::avformat::AVCodecParameters_58;
using internal::avformat::AVCodecParameters_59;
using internal::avformat::AVCodecParameters_60;
using internal::avformat::AVStream_56;
using internal::avformat::AVStream_57;
using internal::avformat::AVStream_58;
using internal::avformat::AVStream_59;
using internal::avformat::AVStream_60;
using ::testing::Return;

template <typename AVStreamType>
void runAVStreamWrapperTestDefaultValues(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avcodec.avcodec_descriptor_get = [](AVCodecID codecID)
  {
    EXPECT_EQ(codecID, ffmpeg::internal::AV_CODEC_ID_NONE);
    return nullptr;
  };

  AVStreamType    stream;
  AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

  EXPECT_EQ(streamWrapper.getIndex(), 0);
  EXPECT_EQ(streamWrapper.getCodecType(), avutil::MediaType::Unknown);
  EXPECT_EQ(streamWrapper.getCodecID(), ffmpeg::internal::AV_CODEC_ID_NONE);
  EXPECT_FALSE(streamWrapper.getCodecDescriptor());
  EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({0, 0}));
  EXPECT_EQ(streamWrapper.getTimeBase(), Rational({0, 0}));
  EXPECT_EQ(streamWrapper.getFrameSize(), Size());
  EXPECT_EQ(streamWrapper.getColorspace(), avutil::ColorSpace::UNSPECIFIED);
  EXPECT_EQ(streamWrapper.getPixelFormat().name, "Unknown");
  EXPECT_TRUE(streamWrapper.getExtradata().empty());
  EXPECT_FALSE(streamWrapper.getCodecParameters());
}

template <typename AVStreamType, typename AVCodecContextType>
void runAVStreamWrapperTestCodecContextSet(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avcodec.avcodec_descriptor_get = [](AVCodecID codecID)
  {
    EXPECT_EQ(codecID, static_cast<AVCodecID>(234));
    return nullptr;
  };

  AVStreamType stream;
  stream.index          = 22;
  stream.time_base      = AVRational({12, 44});
  stream.avg_frame_rate = AVRational({13, 4});

  if constexpr (std::is_same_v<AVStreamType, AVStream_56> ||
                std::is_same_v<AVStreamType, AVStream_57> ||
                std::is_same_v<AVStreamType, AVStream_58>)
  {
    AVCodecContextType codecContext;

    codecContext.codec_type = AVMEDIA_TYPE_AUDIO;
    codecContext.codec_id   = static_cast<AVCodecID>(234);
    codecContext.time_base  = AVRational({7, 22});
    codecContext.width      = 640;
    codecContext.height     = 480;
    codecContext.colorspace = AVCOL_SPC_FCC;
    codecContext.pix_fmt    = static_cast<AVPixelFormat>(444);

    std::array<uint8_t, 3> TEST_EXTRADATA = {88, 99, 120};

    codecContext.extradata      = TEST_EXTRADATA.data();
    codecContext.extradata_size = TEST_EXTRADATA.size();

    stream.codec = reinterpret_cast<AVCodecContext *>(&codecContext);

    AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

    EXPECT_EQ(streamWrapper.getIndex(), 22);
    EXPECT_EQ(streamWrapper.getCodecType(), avutil::MediaType::Audio);
    EXPECT_EQ(streamWrapper.getCodecID(), static_cast<AVCodecID>(234));
    EXPECT_FALSE(streamWrapper.getCodecDescriptor());
    EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({13, 4}));
    EXPECT_EQ(streamWrapper.getTimeBase(), Rational({12, 44}));
    EXPECT_EQ(streamWrapper.getFrameSize(), Size({640, 480}));
    EXPECT_EQ(streamWrapper.getColorspace(), avutil::ColorSpace::FCC);
    EXPECT_EQ(streamWrapper.getExtradata(), dataArrayToByteVector(TEST_EXTRADATA));

    EXPECT_FALSE(streamWrapper.getCodecParameters());
  }
  else
  {
    // The codec context in the stream was removed for newer versions
    AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

    EXPECT_EQ(streamWrapper.getIndex(), 22);
    EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({13, 4}));
    EXPECT_EQ(streamWrapper.getTimeBase(), Rational({12, 44}));
  }
}

template <typename AVStreamType, typename AVCodecParametersType>
void runAVStreamWrapperTestCodecParametersSet(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avcodec.avcodec_descriptor_get = [](AVCodecID codecID)
  {
    EXPECT_EQ(codecID, static_cast<AVCodecID>(234));
    return nullptr;
  };

  AVStreamType stream;
  stream.index          = 22;
  stream.time_base      = AVRational({12, 44});
  stream.avg_frame_rate = AVRational({13, 4});

  AVCodecParametersType codecParameter;

  codecParameter.codec_type  = AVMEDIA_TYPE_AUDIO;
  codecParameter.codec_id    = static_cast<AVCodecID>(234);
  codecParameter.width       = 640;
  codecParameter.height      = 480;
  codecParameter.color_space = AVCOL_SPC_FCC;
  codecParameter.format      = 444;

  std::array<uint8_t, 3> TEST_EXTRADATA = {88, 99, 120};

  codecParameter.extradata      = TEST_EXTRADATA.data();
  codecParameter.extradata_size = TEST_EXTRADATA.size();

  stream.codecpar = reinterpret_cast<AVCodecParameters *>(&codecParameter);

  AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

  EXPECT_EQ(streamWrapper.getIndex(), 22);
  EXPECT_EQ(streamWrapper.getCodecType(), avutil::MediaType::Audio);
  EXPECT_EQ(streamWrapper.getCodecID(), static_cast<AVCodecID>(234));
  EXPECT_FALSE(streamWrapper.getCodecDescriptor());
  EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({13, 4}));
  EXPECT_EQ(streamWrapper.getTimeBase(), Rational({12, 44}));
  EXPECT_EQ(streamWrapper.getFrameSize(), Size({640, 480}));
  EXPECT_EQ(streamWrapper.getColorspace(), avutil::ColorSpace::FCC);
  EXPECT_EQ(streamWrapper.getExtradata(), dataArrayToByteVector(TEST_EXTRADATA));

  EXPECT_TRUE(streamWrapper.getCodecParameters());
}

} // namespace

class AVStreamWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST(AVStreamWrapperTest, shouldThrowIfLibraryNotSet)
{
  AVStream_56                       stream;
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  EXPECT_THROW(AVStreamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries),
               std::runtime_error);
}

TEST(AVStreamWrapperTest, shouldThrowIfAVStreamPointerIsNull)
{
  AVStream_56 *stream{};
  auto         ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  EXPECT_THROW(AVStreamWrapper(reinterpret_cast<AVStream *>(stream), ffmpegLibraries),
               std::runtime_error);
}

TEST_P(AVStreamWrapperTest, TestAVInputFormatWrapperDefaultValues)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVStreamWrapperTestDefaultValues<AVStream_56>(version);
  else if (version.avformat.major == 57)
    runAVStreamWrapperTestDefaultValues<AVStream_57>(version);
  else if (version.avformat.major == 58)
    runAVStreamWrapperTestDefaultValues<AVStream_58>(version);
  else if (version.avformat.major == 59)
    runAVStreamWrapperTestDefaultValues<AVStream_59>(version);
  else if (version.avformat.major == 60)
    runAVStreamWrapperTestDefaultValues<AVStream_60>(version);
}

TEST_P(AVStreamWrapperTest, TestAVInputFormatWrapperWithCodecContextSet)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVStreamWrapperTestCodecContextSet<AVStream_56, AVCodecContext_56>(version);
  else if (version.avformat.major == 57)
    runAVStreamWrapperTestCodecContextSet<AVStream_57, AVCodecContext_57>(version);
  else if (version.avformat.major == 58)
    runAVStreamWrapperTestCodecContextSet<AVStream_58, AVCodecContext_58>(version);
  else if (version.avformat.major == 59)
    runAVStreamWrapperTestCodecContextSet<AVStream_59, AVCodecContext_59>(version);
  else if (version.avformat.major == 60)
    runAVStreamWrapperTestCodecContextSet<AVStream_60, AVCodecContext_60>(version);
}

TEST_P(AVStreamWrapperTest, TestAVInputFormatWrapperWithCodecParametersSet)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    // Version 56 does not have codec parameters yet
    return;
  else if (version.avformat.major == 57)
    runAVStreamWrapperTestCodecParametersSet<AVStream_57, AVCodecParameters_57>(version);
  else if (version.avformat.major == 58)
    runAVStreamWrapperTestCodecParametersSet<AVStream_58, AVCodecParameters_58>(version);
  else if (version.avformat.major == 59)
    runAVStreamWrapperTestCodecParametersSet<AVStream_59, AVCodecParameters_59>(version);
  else if (version.avformat.major == 60)
    runAVStreamWrapperTestCodecParametersSet<AVStream_60, AVCodecParameters_60>(version);
}

INSTANTIATE_TEST_SUITE_P(AVFormatWrappers,
                         AVStreamWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avformat
