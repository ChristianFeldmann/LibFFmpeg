/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecContextWrapper.h>
#include <AVCodec/wrappers/AVCodecContextWrapperInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avcodec
{

using ffmpeg::internal::AVCodecContext;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVColorSpace;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;
using ffmpeg::internal::avcodec::AVCodecContext_56;
using ffmpeg::internal::avcodec::AVCodecContext_57;
using ffmpeg::internal::avcodec::AVCodecContext_58;
using ffmpeg::internal::avcodec::AVCodecContext_59;
using ffmpeg::internal::avcodec::AVCodecContext_60;
using ::testing::Return;

template <typename AVCodecContextType> void runAVCodecContextTest(const LibraryVersions &version)
{
  constexpr auto TEST_CODEC_ID     = AVCodecID(849);
  constexpr auto TEST_PIXEL_FORMAT = AVPixelFormat(289);
  constexpr auto TEST_WIDTH        = 728;
  constexpr auto TEST_HEIGHT       = 20;
  constexpr auto TEST_TIMEBASE     = AVRational({12, 44});

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  int counterAvPixelFormatDescriptorGet       = 0;
  ffmpegLibraries->avutil.av_pix_fmt_desc_get = [&](AVPixelFormat pix_fmt) {
    EXPECT_EQ(pix_fmt, TEST_PIXEL_FORMAT);
    ++counterAvPixelFormatDescriptorGet;
    return nullptr;
  };

  AVCodecContextType codecContext;
  codecContext.codec_type = ffmpeg::internal::AVMEDIA_TYPE_ATTACHMENT;
  codecContext.codec_id   = TEST_CODEC_ID;
  codecContext.pix_fmt    = TEST_PIXEL_FORMAT;
  codecContext.width      = TEST_WIDTH;
  codecContext.height     = TEST_HEIGHT;
  codecContext.colorspace = AVColorSpace::AVCOL_SPC_BT2020_NCL;
  codecContext.time_base  = TEST_TIMEBASE;

  std::array<uint8_t, 4> TEST_EXTRADATA = {22, 56, 19, 22};
  codecContext.extradata                = TEST_EXTRADATA.data();
  codecContext.extradata_size           = static_cast<int>(TEST_EXTRADATA.size());

  AVCodecContextWrapper wrapper(reinterpret_cast<AVCodecContext *>(&codecContext), ffmpegLibraries);

  EXPECT_EQ(wrapper.getCodecType(), avutil::MediaType::Attachment);
  EXPECT_EQ(wrapper.getCodecID(), TEST_CODEC_ID);
  EXPECT_EQ(wrapper.getPixelFormat().name, "None");
  EXPECT_EQ(counterAvPixelFormatDescriptorGet, 1);
  EXPECT_EQ(wrapper.getSize(), Size({TEST_WIDTH, TEST_HEIGHT}));
  EXPECT_EQ(wrapper.getColorspace(), avutil::ColorSpace::BT2020_NCL);
  EXPECT_EQ(wrapper.getTimeBase(), Rational({TEST_TIMEBASE.num, TEST_TIMEBASE.den}));
  EXPECT_EQ(wrapper.getExtradata(), dataArrayToByteVector(TEST_EXTRADATA));

  // Todo: Test opening of files.
}

class AVCodecContextWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_F(AVCodecContextWrapperTest, ConstructorWithNullptrForCodecContextShouldThrow)
{
  auto            ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  AVCodecContext *codecContext    = nullptr;
  EXPECT_THROW(AVCodecContextWrapper wrapper(codecContext, ffmpegLibraries), std::runtime_error);
}

TEST_F(AVCodecContextWrapperTest, ConstructorWithNullptrForFFmpegLibrariesShouldThrow)
{
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;
  AVCodecContext_56                 codecContext;
  EXPECT_THROW(AVCodecContextWrapper wrapper(reinterpret_cast<AVCodecContext *>(&codecContext),
                                             ffmpegLibraries),
               std::runtime_error);
}

TEST_P(AVCodecContextWrapperTest, TestAVCodecContextWrapper)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVCodecContextTest<AVCodecContext_56>(version);
  else if (version.avformat.major == 57)
    runAVCodecContextTest<AVCodecContext_57>(version);
  else if (version.avformat.major == 58)
    runAVCodecContextTest<AVCodecContext_58>(version);
  else if (version.avformat.major == 59)
    runAVCodecContextTest<AVCodecContext_59>(version);
  else if (version.avformat.major == 60)
    runAVCodecContextTest<AVCodecContext_60>(version);
}

INSTANTIATE_TEST_SUITE_P(AVCodecContextWrapper,
                         AVCodecContextWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avcodec
