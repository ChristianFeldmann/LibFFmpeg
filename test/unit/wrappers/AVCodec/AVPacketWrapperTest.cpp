/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVCodec/VersionToAVCodecTypes.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

#include <array>

namespace libffmpeg::avcodec
{

namespace
{

using libffmpeg::internal::AVPacket;
using ::testing::NiceMock;
using ::testing::Return;

constexpr auto         TEST_STREAM_INDEX = 7;
constexpr auto         TEST_PTS          = 7338;
constexpr auto         TEST_DTS          = 378;
constexpr auto         TEST_DURATION     = 6789;
constexpr auto         TEST_FLAGS        = 0b101;
std::array<uint8_t, 3> TEST_DATA         = {88, 99, 120};

void checkPacketForExpectedDefaultValues(const AVPacketWrapper &packet)
{
  EXPECT_EQ(packet.getStreamIndex(), TEST_STREAM_INDEX);
  EXPECT_EQ(packet.getPTS(), TEST_PTS);
  EXPECT_EQ(packet.getDTS(), TEST_DTS);
  EXPECT_EQ(packet.getDuration(), TEST_DURATION);
  EXPECT_TRUE(packet.getFlags().keyframe);
  EXPECT_FALSE(packet.getFlags().corrupt);
  EXPECT_TRUE(packet.getFlags().discard);
};

template <typename AVPacketType>
class FFmpegLibrariesMockWithPacketAllocation : public FFmpegLibrariesMock
{
public:
  FFmpegLibrariesMockWithPacketAllocation() : FFmpegLibrariesMock()
  {
    this->avcodec.av_packet_alloc = [this]() { return this->allocatePacket(); };
    this->avcodec.av_packet_free  = [this](AVPacket **packet) { this->freePacket(packet); };
    this->avcodec.av_new_packet   = [this](AVPacket *packet, int dataSize)
    { return this->newPacket(packet, dataSize); };
    if constexpr (std::is_same_v<AVPacketType, libffmpeg::internal::avcodec::AVPacket_56>)
    {
      this->avcodec.av_init_packet = [this](AVPacket *packet) { this->initPacket(packet); };
      this->avcodec.av_free_packet = [this](AVPacket *packet) { this->freePacket(packet); };
    }
  };

  ~FFmpegLibrariesMockWithPacketAllocation()
  {
    EXPECT_EQ(this->packetCounters.packetAllocationCounter, this->packetCounters.packetFreeCounter)
        << "Not all allocated packets were freed again";
    // While it is not technically required for "init" to be called if a packet is allocated in
    // AVCodec major version 56, the AVPacketWrapper will always do so.
    EXPECT_EQ(this->packetCounters.packetInitCounter, this->packetCounters.freePacketCounter)
        << "Not all allocated packets were freed again";
    EXPECT_EQ(this->packetCounters.packetNewCounter, this->packetCounters.dataDeletionCounter)
        << "Not all allocated packet data was freed again";
  }

  AVPacket *allocatePacket()
  {
    auto packet          = new AVPacketType;
    packet->stream_index = TEST_STREAM_INDEX;
    packet->pts          = TEST_PTS;
    packet->dts          = TEST_DTS;
    packet->duration     = TEST_DURATION;
    packet->flags        = TEST_FLAGS;
    packet->size         = 0;
    packet->data         = nullptr;
    ++this->packetCounters.packetAllocationCounter;
    return reinterpret_cast<AVPacket *>(packet);
  }

  void freePacket(AVPacket **packet)
  {
    auto actualPacket = reinterpret_cast<AVPacketType *>(*packet);
    if (actualPacket->data != nullptr && actualPacket->size > 0)
    {
      delete[] actualPacket->data;
      ++this->packetCounters.dataDeletionCounter;
    }
    delete actualPacket;
    packet = nullptr;
    ++this->packetCounters.packetFreeCounter;
  }

  int newPacket(AVPacket *packet, int dataSize)
  {
    auto actualPacket  = reinterpret_cast<AVPacketType *>(packet);
    actualPacket->data = new uint8_t[dataSize];
    actualPacket->size = dataSize;
    ++this->packetCounters.packetNewCounter;
    return 0;
  };

  // AVCodec major version 56 only
  void initPacket(AVPacket *packet)
  {
    auto castPacket = reinterpret_cast<libffmpeg::internal::avcodec::AVPacket_56 *>(packet);
    castPacket->stream_index = TEST_STREAM_INDEX;
    castPacket->pts          = TEST_PTS;
    castPacket->dts          = TEST_DTS;
    castPacket->duration     = TEST_DURATION;
    castPacket->flags        = TEST_FLAGS;
    castPacket->size         = 0;
    castPacket->data         = nullptr;
    ++this->packetCounters.packetInitCounter;
  }

  // AVCodec major version 56 only
  void freePacket(AVPacket *packet)
  {
    auto actualPacket = reinterpret_cast<AVPacketType *>(packet);
    if (actualPacket->data != nullptr && actualPacket->size > 0)
    {
      delete[] actualPacket->data;
      ++this->packetCounters.dataDeletionCounter;
    }
    ++this->packetCounters.freePacketCounter;
  }

  struct PacketCounters
  {
    int packetAllocationCounter{0};
    int dataDeletionCounter{0};
    int packetFreeCounter{0};
    int packetNewCounter{0};
    // These are AVCodec major version 56 only
    int packetInitCounter{0};
    int freePacketCounter{0};
  };
  PacketCounters packetCounters{};
};

template <FFmpegVersion V> void runEmptyConstructionTest()
{
  const auto version = getLibraryVerions(V);

  auto ffmpegLibraries =
      std::make_shared<FFmpegLibrariesMockWithPacketAllocation<AVPacketType<V>>>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  {
    AVPacketWrapper packet(ffmpegLibraries);

    checkPacketForExpectedDefaultValues(packet);
    EXPECT_EQ(packet.getDataSize(), 0);

    if (version.avcodec.major > 56)
      EXPECT_EQ(ffmpegLibraries->packetCounters.packetAllocationCounter, 1);
    else
      EXPECT_EQ(ffmpegLibraries->packetCounters.packetInitCounter, 1);
    EXPECT_EQ(ffmpegLibraries->packetCounters.packetFreeCounter, 0);
    EXPECT_EQ(ffmpegLibraries->packetCounters.freePacketCounter, 0);
  }

  // No data should have been allocated with this constructor
  EXPECT_EQ(ffmpegLibraries->packetCounters.dataDeletionCounter, 0);
}

template <FFmpegVersion V> void runConstructorFromDataTest()
{
  const auto version = getLibraryVerions(V);

  auto ffmpegLibraries =
      std::make_shared<FFmpegLibrariesMockWithPacketAllocation<AVPacketType<V>>>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  {
    const auto      data = dataArrayToByteVector(TEST_DATA);
    AVPacketWrapper packet(data, ffmpegLibraries);

    checkPacketForExpectedDefaultValues(packet);
    EXPECT_EQ(packet.getDataSize(), data.size());
    EXPECT_EQ(packet.getData(), data);

    if (version.avcodec.major > 56)
      EXPECT_EQ(ffmpegLibraries->packetCounters.packetAllocationCounter, 1);
    else
      EXPECT_EQ(ffmpegLibraries->packetCounters.packetInitCounter, 1);
    EXPECT_EQ(ffmpegLibraries->packetCounters.packetFreeCounter, 0);
    EXPECT_EQ(ffmpegLibraries->packetCounters.freePacketCounter, 0);
  }

  EXPECT_EQ(ffmpegLibraries->packetCounters.dataDeletionCounter, 1);
}

template <FFmpegVersion V> void runTimestampTest()
{
  const auto version = getLibraryVerions(V);

  auto ffmpegLibraries =
      std::make_shared<FFmpegLibrariesMockWithPacketAllocation<AVPacketType<V>>>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  AVPacketWrapper packet(ffmpegLibraries);
  checkPacketForExpectedDefaultValues(packet);

  constexpr int64_t PTS_TEST = 890;
  constexpr int64_t DTS_TEST = 8200;

  packet.setTimestamps(DTS_TEST, PTS_TEST);
  EXPECT_EQ(packet.getPTS(), PTS_TEST);
  EXPECT_EQ(packet.getDTS(), DTS_TEST);

  packet.setTimestamps(DTS_TEST, internal::AV_NOPTS_VALUE);
  EXPECT_FALSE(packet.getPTS());
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
  auto ffmpegLibraries = std::make_shared<NiceMock<FFmpegLibrariesMock>>();

  ffmpegLibraries->avcodec.av_packet_alloc = []() { return nullptr; };

  EXPECT_THROW(AVPacketWrapper packet(ffmpegLibraries), std::runtime_error);

  const ByteVector data = {std::byte(0x00), std::byte(0xff)};
  EXPECT_THROW(AVPacketWrapper packet(data, ffmpegLibraries), std::runtime_error);
}

TEST_P(AVPacketWrapperTest, TestDefaultConstructor)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runEmptyConstructionTest);
}

TEST_P(AVPacketWrapperTest, TestConstructorFromData)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runConstructorFromDataTest);
}

TEST_P(AVPacketWrapperTest, TestSettingOfTimestamp)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runTimestampTest);
}

INSTANTIATE_TEST_SUITE_P(AVCodecWrappers,
                         AVPacketWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace libffmpeg::avcodec
