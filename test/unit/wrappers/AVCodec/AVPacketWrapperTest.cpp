/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <common/InternalTypes.h>
#include <wrappers/AVCodec/AVPacketWrapper.h>
#include <wrappers/AVCodec/AVPacketWrapperInternal.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avcodec
{

namespace
{

using ffmpeg::internal::AVPacket;
using ffmpeg::internal::avcodec::AVPacket_56;
using ffmpeg::internal::avcodec::AVPacket_57;
using ffmpeg::internal::avcodec::AVPacket_58;
using ffmpeg::internal::avcodec::AVPacket_59;
using ffmpeg::internal::avcodec::AVPacket_60;
using ::testing::Return;

template <typename AVPacketType> void runAVPacketWrapperTest(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  constexpr auto         TEST_STREAM_INDEX = 7;
  constexpr auto         TEST_PTS          = 7338;
  constexpr auto         TEST_DTS          = 378;
  constexpr auto         TEST_DURATION     = 6789;
  constexpr auto         TEST_FLAGS        = 0b101;
  std::array<uint8_t, 3> TEST_DATA         = {88, 99, 120};

  auto checkForExpectedPacketDefaultValues = [&](const AVPacketWrapper &packet)
  {
    EXPECT_EQ(packet.getStreamIndex(), TEST_STREAM_INDEX);
    EXPECT_EQ(packet.getPTS(), TEST_PTS);
    EXPECT_EQ(packet.getDTS(), TEST_DTS);
    EXPECT_EQ(packet.getDuration(), TEST_DURATION);
    EXPECT_TRUE(packet.getFlags().keyframe);
    EXPECT_FALSE(packet.getFlags().corrupt);
    EXPECT_TRUE(packet.getFlags().discard);
  };

  int packetAllocationCounter              = 0;
  ffmpegLibraries->avcodec.av_packet_alloc = [&packetAllocationCounter, &TEST_DATA]()
  {
    auto packet          = new AVPacketType;
    packet->stream_index = TEST_STREAM_INDEX;
    packet->pts          = TEST_PTS;
    packet->dts          = TEST_DTS;
    packet->duration     = TEST_DURATION;
    packet->flags        = TEST_FLAGS;
    packet->size         = 0;
    packet->data         = nullptr;
    packetAllocationCounter++;
    return reinterpret_cast<AVPacket *>(packet);
  };

  int packetFreeCounter   = 0;
  int dataDeletionCounter = 0;
  ffmpegLibraries->avcodec.av_packet_free =
      [&packetFreeCounter, &dataDeletionCounter](AVPacket **packet)
  {
    auto actualPacket = reinterpret_cast<AVPacketType *>(*packet);
    if (actualPacket->data != nullptr && actualPacket->size > 0)
    {
      delete[] actualPacket->data;
      dataDeletionCounter++;
    }
    delete actualPacket;
    packet = nullptr;
    packetFreeCounter++;
  };

  {
    AVPacketWrapper packet(ffmpegLibraries);

    checkForExpectedPacketDefaultValues(packet);
    EXPECT_EQ(packet.getDataSize(), 0);

    EXPECT_EQ(packetAllocationCounter, 1);
    EXPECT_EQ(packetFreeCounter, 0);
  }

  EXPECT_EQ(packetAllocationCounter, 1);
  EXPECT_EQ(packetFreeCounter, 1);
  EXPECT_EQ(dataDeletionCounter, 0);

  int packetNewCounter                   = 0;
  ffmpegLibraries->avcodec.av_new_packet = [](AVPacket *packet, int dataSize)
  {
    auto actualPacket  = reinterpret_cast<AVPacketType *>(packet);
    actualPacket->data = new uint8_t[dataSize];
    actualPacket->size = dataSize;
    return 0;
  };

  {
    const auto      data = dataArrayToByteVector(TEST_DATA);
    AVPacketWrapper packet(data, ffmpegLibraries);

    checkForExpectedPacketDefaultValues(packet);
    EXPECT_EQ(packet.getDataSize(), data.size());
    EXPECT_EQ(packet.getData(), data);

    EXPECT_EQ(packetAllocationCounter, 2);
    EXPECT_EQ(packetFreeCounter, 1);
  }

  EXPECT_EQ(packetAllocationCounter, 2);
  EXPECT_EQ(packetFreeCounter, 2);
  EXPECT_EQ(dataDeletionCounter, 1);
}

} // namespace

class AVPacketWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_F(AVPacketWrapperTest, ConstructorWithNullptrForFFmpegLibrariesShouldThrow)
{
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;
  EXPECT_THROW(AVPacketWrapper packet(ffmpegLibraries), std::runtime_error);
}

TEST_F(AVPacketWrapperTest, IfPacketAllocationFailsShouldThrow)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  ffmpegLibraries->avcodec.av_packet_alloc = []() { return nullptr; };

  EXPECT_THROW(AVPacketWrapper packet(ffmpegLibraries), std::runtime_error);

  const ByteVector data = {std::byte(0x00), std::byte(0xff)};
  EXPECT_THROW(AVPacketWrapper packet(data, ffmpegLibraries), std::runtime_error);
}

TEST_P(AVPacketWrapperTest, TestAVInputFormatWrapperWithCodecParametersSet)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVPacketWrapperTest<AVPacket_56>(version);
  else if (version.avformat.major == 57)
    runAVPacketWrapperTest<AVPacket_57>(version);
  else if (version.avformat.major == 58)
    runAVPacketWrapperTest<AVPacket_58>(version);
  else if (version.avformat.major == 59)
    runAVPacketWrapperTest<AVPacket_59>(version);
  else if (version.avformat.major == 60)
    runAVPacketWrapperTest<AVPacket_60>(version);
}

INSTANTIATE_TEST_SUITE_P(AVCodecWrappers,
                         AVPacketWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avcodec
