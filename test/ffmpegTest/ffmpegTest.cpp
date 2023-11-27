/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Decoder.h>
#include <Demuxer.h>
#include <common/ComparisonFunctions.h>
#include <common/Functions.h>
#include <libHandling/FFmpegLibrariesInterfaceBuilder.h>
#include <libHandling/libraryFunctions/Functions.h>

#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ffmpeg;

namespace
{

// Generated with
// ffmpeg -y -f lavfi -i testsrc -f lavfi -i "sine=frequency=1000:duration=60" -f lavfi -i
// "sine=frequency=600:duration=60" -filter_complex amerge -t 1 -pix_fmt yuv420p
// TestFile_h264_aac_1s_320x240.mp4
constexpr auto TEST_FILE_NAME = "TestFile_h264_aac_1s_320x240.mp4";

std::shared_ptr<FFmpegLibrariesInterface> openLibraries()
{
  const auto loadingResult =
      FFmpegLibrariesInterfaceBuilder().withAdditionalSearchPaths({"."}).tryLoadingOfLibraries();
  EXPECT_TRUE(loadingResult) << "Error loading libraries";
  return loadingResult.librariesInterface;
}

Demuxer openTestFileInDemuxer(std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
{
  Demuxer demuxer(librariesInterface);
  const auto [openSuccessfull, openingLog] = demuxer.openFile(TEST_FILE_NAME);
  EXPECT_TRUE(openSuccessfull) << "Opening test file " << TEST_FILE_NAME << " failed.";

  return demuxer;
}

} // namespace

TEST(FFmpegTest, LoadLibrariesAndLogVersion)
{
  const auto loadingResult =
      FFmpegLibrariesInterfaceBuilder().withAdditionalSearchPaths({"."}).tryLoadingOfLibraries();

  ASSERT_TRUE(loadingResult) << "Error loading ffmpeg library: " << loadingResult.errorMessage
                             << "\nLog:\n"
                             << to_string(loadingResult.loadingLog, ConcatenationSymbol::Newline);

  const auto librariesInfo = loadingResult.librariesInterface->getLibrariesInfo();
  EXPECT_EQ(librariesInfo.size(), 4);

  for (const auto &libInfo : librariesInfo)
    std::cerr << "[          ] [INFO] Lib " << libInfo.name << " loaded from path " << libInfo.path
              << " with version " << libInfo.version << "\n";
}

TEST(FFmpegTest, CheckFormatAndStreamParameters)
{
  auto       demuxer       = openTestFileInDemuxer(openLibraries());
  const auto formatContext = demuxer.getFormatContext();

  EXPECT_EQ(formatContext->getStartTime(), 0);
  EXPECT_EQ(formatContext->getDuration(), 1000000);
  EXPECT_EQ(formatContext->getNumberStreams(), 2);

  const auto metadataMap = formatContext->getMetadata().toMap();
  EXPECT_EQ(metadataMap.size(), 4);
  EXPECT_EQ(metadataMap.count("compatible_brands"), 1);
  EXPECT_EQ(metadataMap.at("compatible_brands"), "isomiso2avc1mp41");
  EXPECT_EQ(metadataMap.count("encoder"), 1);
  EXPECT_EQ(metadataMap.at("encoder"), "Lavf59.16.100");
  EXPECT_EQ(metadataMap.count("major_brand"), 1);
  EXPECT_EQ(metadataMap.at("major_brand"), "isom");
  EXPECT_EQ(metadataMap.count("minor_version"), 1);
  EXPECT_EQ(metadataMap.at("minor_version"), "512");

  const auto inputFormat = formatContext->getInputFormat();
  EXPECT_EQ(inputFormat.getName(), "mov,mp4,m4a,3gp,3g2,mj2");
  EXPECT_EQ(inputFormat.getLongName(), "QuickTime / MOV");
  EXPECT_THAT((std::array{"mov,mp4,m4a,3gp,3g2,mj2,psp,m4b,ism,ismv,isma,f4v",
                          "mov,mp4,m4a,3gp,3g2,mj2,psp,m4b,ism,ismv,isma,f4v,avif"}),
              testing::Contains(inputFormat.getExtensions()));

  EXPECT_EQ(inputFormat.getMimeType(), "");
  const auto                   flags = inputFormat.getFlags();
  avformat::AVInputFormatFlags expectedFlags{};
  expectedFlags.showIDs    = true;
  expectedFlags.noByteSeek = true;
  expectedFlags.seekToPTS  = true;
  EXPECT_EQ(flags, expectedFlags);

  const auto &audioStream = formatContext->getStream(0);
  EXPECT_EQ(audioStream.getCodecType(), MediaType::Audio);

  const auto audioCodecDescriptor = audioStream.getCodecDescriptor();
  EXPECT_EQ(audioCodecDescriptor.getMediaType(), MediaType::Audio);
  EXPECT_EQ(audioCodecDescriptor.getCodecName(), "aac");
  EXPECT_EQ(audioCodecDescriptor.getLongName(), "AAC (Advanced Audio Coding)");
  avcodec::AVCodecDescriptorProperties expectedAudioProperties{};
  expectedAudioProperties.intraOnly = true;
  expectedAudioProperties.lossy     = true;
  EXPECT_EQ(audioCodecDescriptor.getProperties(), expectedAudioProperties);
  EXPECT_EQ(audioCodecDescriptor.getMimeTypes().size(), 0);
  EXPECT_EQ(audioCodecDescriptor.getProfiles().size(), 8);
  EXPECT_TRUE(areEqual(audioCodecDescriptor.getProfiles(),
                       {"LC", "HE-AAC", "HE-AACv2", "LD", "ELD", "Main", "SSR", "LTP"}));
  EXPECT_EQ(audioStream.getAverageFrameRate(), Rational({24, 1}));
  EXPECT_EQ(audioStream.getTimeBase(), Rational({1, 44100}));
  EXPECT_EQ(audioStream.getFrameSize(), Size({0, 0}));
  EXPECT_EQ(audioStream.getColorspace(), ColorSpace::UNSPECIFIED);
  EXPECT_EQ(audioStream.getExtradata().size(), 5);
  EXPECT_EQ(audioStream.getIndex(), 0);

  const auto &videoStream = formatContext->getStream(1);
  EXPECT_EQ(videoStream.getCodecType(), MediaType::Video);

  const auto videoCodecDescriptor = videoStream.getCodecDescriptor();
  EXPECT_EQ(videoCodecDescriptor.getMediaType(), MediaType::Video);
  EXPECT_EQ(videoCodecDescriptor.getCodecName(), "h264");
  EXPECT_EQ(videoCodecDescriptor.getLongName(), "H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10");
  avcodec::AVCodecDescriptorProperties expectedVideoProperties{};
  expectedVideoProperties.lossy    = true;
  expectedVideoProperties.lossless = true;
  expectedVideoProperties.reorder  = true;
  EXPECT_EQ(videoCodecDescriptor.getProperties(), expectedVideoProperties);
  EXPECT_EQ(videoCodecDescriptor.getMimeTypes().size(), 0);
  EXPECT_EQ(videoCodecDescriptor.getProfiles().size(), 15);
  EXPECT_TRUE(areEqual(videoCodecDescriptor.getProfiles(),
                       {"Baseline",
                        "Constrained Baseline",
                        "Main",
                        "Extended",
                        "High",
                        "High 10",
                        "High 10 Intra",
                        "High 4:2:2",
                        "High 4:2:2 Intra",
                        "High 4:4:4",
                        "High 4:4:4 Predictive",
                        "High 4:4:4 Intra",
                        "CAVLC 4:4:4",
                        "Multiview High",
                        "Stereo High"}));
  EXPECT_EQ(videoStream.getAverageFrameRate(), Rational({25, 1}));
  EXPECT_EQ(videoStream.getTimeBase(), Rational({1, 12800}));
  EXPECT_EQ(videoStream.getFrameSize(), Size({320, 240}));
  EXPECT_EQ(videoStream.getColorspace(), ColorSpace::UNSPECIFIED);
  EXPECT_EQ(videoStream.getPixelFormat().name, "yuv420p");
  EXPECT_EQ(videoStream.getExtradata().size(), 46);
  EXPECT_EQ(videoStream.getIndex(), 1);
}

TEST(FFmpegTest, DemuxPackets)
{
  auto       demuxer       = openTestFileInDemuxer(openLibraries());
  const auto formatContext = demuxer.getFormatContext();

  constexpr std::array<int, 25> expectedDataSizesVideo = {3827, 499, 90,  46, 29,  439, 82, 36, 40,
                                                          461,  56,  36,  25, 419, 54,  29, 23, 339,
                                                          50,   46,  287, 54, 30,  171, 141};
  constexpr std::array<int, 45> expectedDataSizesAudio = {
      343, 377, 258, 299, 304, 339, 316, 362, 353, 358, 385, 358, 348, 365, 358,
      364, 369, 385, 339, 377, 348, 368, 370, 367, 353, 348, 397, 391, 410, 337,
      372, 370, 372, 342, 431, 378, 370, 333, 355, 366, 355, 388, 412, 349, 7};

  int packetCountAudio = 0;
  int packetCountVideo = 0;
  while (auto packet = demuxer.getNextPacket())
  {
    EXPECT_TRUE(packet.getStreamIndex() == 0 || packet.getStreamIndex() == 1);
    if (packet.getStreamIndex() == 0)
    {
      EXPECT_EQ(packet.getDataSize(), expectedDataSizesAudio.at(packetCountAudio));
      ++packetCountAudio;
    }
    else if (packet.getStreamIndex() == 1)
    {
      EXPECT_EQ(packet.getDataSize(), expectedDataSizesVideo.at(packetCountVideo));
      ++packetCountVideo;
    }
  }

  EXPECT_EQ(packetCountAudio, 45);
  EXPECT_EQ(packetCountVideo, 25);
}

TEST(FFmpegTest, DecodingTest)
{
  auto librariesInterface = openLibraries();
  auto demuxer            = openTestFileInDemuxer(librariesInterface);
  auto decoder            = Decoder(librariesInterface);

  const auto streamToDecode = 1;
  const auto stream         = demuxer.getFormatContext()->getStream(streamToDecode);

  decoder.openForDecoding(stream);

  auto frameCounter = 0;
  while (const auto packet = demuxer.getNextPacket())
  {
    if (packet.getStreamIndex() != streamToDecode)
      continue;

    EXPECT_EQ(decoder.getDecoderState(), Decoder::State::NeedsMoreData);

    auto result = decoder.pushPacket(packet);
    ASSERT_NE(result, Decoder::PushResult::Error);

    if (decoder.getDecoderState() == Decoder::State::RetrieveFrames)
    {
      while (decoder.getDecoderState() == Decoder::State::RetrieveFrames)
      {
        const auto frame = decoder.decodeNextFrame();
        EXPECT_TRUE(frame);

        EXPECT_EQ(frame->getSize(), Size({320, 240}));
        EXPECT_EQ(frame->getLineSize(0), 320);
        //EXPECT_EQ(frame->getPictType(), );
        // Check some more values

        ++frameCounter;
      }

      const auto frame = decoder.decodeNextFrame();
      EXPECT_FALSE(frame);
    }

    if (result == Decoder::PushResult::NotPushedPullFramesFirst)
    {
      result = decoder.pushPacket(packet);
      EXPECT_EQ(result, Decoder::PushResult::Ok);
    }
  }

  decoder.setFlushing();

  EXPECT_EQ(decoder.getDecoderState(), Decoder::State::RetrieveFrames);
  while (decoder.getDecoderState() == Decoder::State::RetrieveFrames)
  {
    const auto frame = decoder.decodeNextFrame();
    EXPECT_TRUE(frame);

    EXPECT_EQ(frame->getSize(), Size({320, 240}));
    // Check some more values

    ++frameCounter;
  }

  EXPECT_EQ(decoder.getDecoderState(), Decoder::State::EndOfBitstream);
}
