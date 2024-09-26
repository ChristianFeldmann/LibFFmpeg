/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <common/ComparisonFunctions.h>

#include "LibrariesWithLogging.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace libffmpeg::test::integration
{

using libffmpeg::avutil::PictureType;
using ::testing::Contains;

TEST(FFmpegTest, DecodingTest)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto demuxer = libsAndLogs.openTestFileInDemuxer();
  auto decoder = Decoder(libsAndLogs.libraries);

  const auto streamToDecode      = 1;
  const auto stream              = demuxer.getFormatContext()->getStream(streamToDecode);
  const auto avcodecVersionMajor = libsAndLogs.libraries->getLibrariesVersion().avcodec.major;

  // The amount of padding that FFmpeg uses depends on how it was compiled.
  // Here, we use our own compiled versions. But in general this can not be predicted.
  constexpr std::array<int, 2> EXPECTED_LINESIZE_LUMA   = {320, 384};
  constexpr std::array<int, 2> EXPECTED_LINESIZE_CHROMA = {160, 192};

  ASSERT_TRUE(decoder.openForDecoding(stream));

  auto totalFrameCounter = 0;

  auto pullFramesFromDecoder = [&decoder,
                                &totalFrameCounter,
                                &EXPECTED_LINESIZE_LUMA,
                                &EXPECTED_LINESIZE_CHROMA,
                                &avcodecVersionMajor]()
  {
    int framesDecodedInLoop = 0;
    while (const auto frame = decoder.decodeNextFrame())
    {
      EXPECT_TRUE(frame);

      EXPECT_EQ(frame->getPixelFormatDescriptor().name, "yuv420p");

      EXPECT_EQ(frame->getSize(), Size({320, 240}));
      EXPECT_THAT(EXPECTED_LINESIZE_LUMA, Contains(frame->getLineSize(0)));
      EXPECT_THAT(EXPECTED_LINESIZE_CHROMA, Contains(frame->getLineSize(1)));
      EXPECT_THAT(EXPECTED_LINESIZE_CHROMA, Contains(frame->getLineSize(2)));
      EXPECT_EQ(frame->getSampleAspectRatio(), Rational({1, 1}));

      const auto absoluteFrameIndex = totalFrameCounter + framesDecodedInLoop;

      constexpr std::array<PictureType, 25> expectedPictureTypes = {
          PictureType::I, PictureType::B, PictureType::B, PictureType::B, PictureType::P,
          PictureType::B, PictureType::B, PictureType::B, PictureType::P, PictureType::B,
          PictureType::B, PictureType::B, PictureType::P, PictureType::B, PictureType::B,
          PictureType::B, PictureType::P, PictureType::B, PictureType::B, PictureType::P,
          PictureType::B, PictureType::B, PictureType::P, PictureType::P, PictureType::P};
      EXPECT_EQ(frame->getPictType(), expectedPictureTypes.at(absoluteFrameIndex));

      constexpr std::array<int64_t, 25> expectedPTSValues = {
          0,    512,  1024, 1536, 2048, 2560, 3072, 3584,  4096,  4608,  5120,  5632, 6144,
          6656, 7168, 7680, 8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288};
      if (avcodecVersionMajor == 56)
        EXPECT_FALSE(frame->getPTS());
      else
        EXPECT_EQ(frame->getPTS(), expectedPTSValues.at(absoluteFrameIndex));

      constexpr std::array<std::int64_t, 25> expectedFrameHashes = {
          -3007600926112186533, -3391200051889142087, -1160633604618185881, 4273627566792723166,
          -4154039706688151037, -1301428058581480615, 7452780297656341798,  7600256964838247071,
          -8499296325100989937, -8234365708930177171, -6091801175307918277, -6243952166556384038,
          -1538307937175716804, -1214069488220656158, 221111691894747256,   -6002299333845183392,
          2227144204187938134,  6419852838956731569,  -2032560420377261054, 84234931995669811,
          -6767048039990945080, -8993409000448418032, -7272467650392102601, 3844105725739243304,
          -2350406183677959349};
      EXPECT_EQ(calculateFrameDataHash(*frame), expectedFrameHashes.at(absoluteFrameIndex));

      ++framesDecodedInLoop;
    }
    return framesDecodedInLoop;
  };

  while (const auto packet = demuxer.getNextPacket())
  {
    if (packet->getStreamIndex() != streamToDecode)
      continue;

    EXPECT_EQ(decoder.getDecoderState(), Decoder::State::NeedsMoreData);

    auto result = decoder.sendPacket(*packet);
    ASSERT_NE(result, Decoder::SendPacketResult::Error);

    if (decoder.getDecoderState() == Decoder::State::RetrieveFrames)
    {
      const auto framesDecoded = pullFramesFromDecoder();
      EXPECT_GT(framesDecoded, 0);
      totalFrameCounter += framesDecoded;
      EXPECT_EQ(decoder.getDecoderState(), Decoder::State::NeedsMoreData);
    }

    if (result == Decoder::SendPacketResult::NotSentPullFramesFirst)
    {
      result = decoder.sendPacket(*packet);
      EXPECT_EQ(result, Decoder::SendPacketResult::Ok);
    }
  }

  decoder.setFlushing();

  EXPECT_EQ(decoder.getDecoderState(), Decoder::State::RetrieveFrames);
  const auto framesDecoded = pullFramesFromDecoder();
  EXPECT_GT(framesDecoded, 0);
  totalFrameCounter += framesDecoded;
  EXPECT_EQ(decoder.getDecoderState(), Decoder::State::EndOfBitstream);

  EXPECT_EQ(totalFrameCounter, 25);
}

} // namespace libffmpeg::test::integration
