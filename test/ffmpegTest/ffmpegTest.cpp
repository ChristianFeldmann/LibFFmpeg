/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Demuxer.h>
#include <common/Functions.h>
#include <libHandling/FFmpegLibrariesInterfaceBuilder.h>
#include <libHandling/libraryFunctions/Functions.h>

#include <array>

#include <gtest/gtest.h>

using namespace ffmpeg;

// Generated with
// ffmpeg -y -f lavfi -i testsrc -f lavfi -i "sine=frequency=1000:duration=60" -f lavfi -i
// "sine=frequency=600:duration=60" -filter_complex amerge -t 1 -pix_fmt yuv420p
// TestFile_h264_aac_1s_320x240.mp4
constexpr auto TEST_FILE_NAME = "TestFile_h264_aac_1s_320x240.mp4";

TEST(FFmpegTest, LoadLibrariesAndLogVersion)
{
  const auto loadingResult = FFmpegLibrariesInterfaceBuilder().tryLoadingOfLibraries();

  EXPECT_TRUE(loadingResult) << "Error loading ffmpeg library: " << loadingResult.errorMessage
                             << "\nLog:\n"
                             << functions::to_string(loadingResult.loadingLog,
                                                     functions::ConcatenationSymbol::Newline);

  for (const auto &libInfo : loadingResult.librariesInterface->getLibrariesInfo())
    std::cerr << "[          ] [INFO] Lib " << libInfo.name << " loaded from path " << libInfo.path
              << " with version " << libInfo.version << "\n";
}

TEST(FFmpegTest, CheckFormatAndStreamParameters)
{
  const auto loadingResult = FFmpegLibrariesInterfaceBuilder().tryLoadingOfLibraries();
  EXPECT_TRUE(loadingResult) << "Error loading libraries";

  Demuxer demuxer(loadingResult.librariesInterface);
  const auto [openSuccessfull, openingLog] = demuxer.openFile(TEST_FILE_NAME);
  EXPECT_TRUE(openSuccessfull) << "Opening test file " << TEST_FILE_NAME << " failed.";

  auto formatContext = demuxer.getFormatContext();
  EXPECT_EQ(formatContext.getStartTime(), 0);
  EXPECT_EQ(formatContext.getDuration(), 1000000);
  EXPECT_EQ(formatContext.getNumberStreams(), 2);

  const auto metadataMap = formatContext.getMetadata().toMap();
  EXPECT_EQ(metadataMap.size(), 4);
  EXPECT_EQ(metadataMap.count("compatible_brands"), 1);
  EXPECT_EQ(metadataMap.at("compatible_brands"), "isomiso2avc1mp41");
  EXPECT_EQ(metadataMap.count("encoder"), 1);
  EXPECT_EQ(metadataMap.at("encoder"), "Lavf59.16.100");
  EXPECT_EQ(metadataMap.count("major_brand"), 1);
  EXPECT_EQ(metadataMap.at("major_brand"), "isom");
  EXPECT_EQ(metadataMap.count("minor_version"), 1);
  EXPECT_EQ(metadataMap.at("minor_version"), "512");

  const auto inputFormat = formatContext.getInputFormat();
  EXPECT_EQ(inputFormat.getName(), "mov,mp4,m4a,3gp,3g2,mj2");
  EXPECT_EQ(inputFormat.getLongName(), "QuickTime / MOV");
  EXPECT_EQ(inputFormat.getExtensions(), "mov,mp4,m4a,3gp,3g2,mj2,psp,m4b,ism,ismv,isma,f4v");
  EXPECT_EQ(inputFormat.getMimeType(), "");
  const auto                   flags = inputFormat.getFlags();
  avformat::AVInputFormatFlags expectedFlags{};
  expectedFlags.showIDs    = true;
  expectedFlags.noByteSeek = true;
  expectedFlags.seekToPTS  = true;
  EXPECT_EQ(flags, expectedFlags);

  const auto &audioStream = formatContext.getStream(0);
  EXPECT_EQ(audioStream.getCodecType(), AVMEDIA_TYPE_AUDIO);
  EXPECT_EQ(audioStream.getCodecTypeName(), "Audio");

  const auto audioCodecDescriptor = audioStream.getCodecDescriptor();
  EXPECT_EQ(audioCodecDescriptor.getMediaType(), AVMEDIA_TYPE_AUDIO);
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
  EXPECT_EQ(audioStream.getAverageFrameRate(), AVRational({24, 1}));
  EXPECT_EQ(audioStream.getTimeBase(), AVRational({1, 44100}));
  EXPECT_EQ(audioStream.getFrameSize(), Size({0, 0}));
  EXPECT_EQ(audioStream.getColorspace(), ColorSpace::UNSPECIFIED);
  EXPECT_EQ(audioStream.getPixelFormat().name, "gray");
  EXPECT_EQ(audioStream.getExtradata().size(), 5);
  EXPECT_EQ(audioStream.getIndex(), 0);

  const auto &videoStream = formatContext.getStream(1);
  EXPECT_EQ(videoStream.getCodecType(), AVMEDIA_TYPE_VIDEO);
  EXPECT_EQ(videoStream.getCodecTypeName(), "Video");

  const auto videoCodecDescriptor = videoStream.getCodecDescriptor();
  EXPECT_EQ(videoCodecDescriptor.getMediaType(), AVMEDIA_TYPE_VIDEO);
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
  EXPECT_EQ(videoStream.getAverageFrameRate(), AVRational({25, 1}));
  EXPECT_EQ(videoStream.getTimeBase(), AVRational({1, 12800}));
  EXPECT_EQ(videoStream.getFrameSize(), Size({320, 240}));
  EXPECT_EQ(videoStream.getColorspace(), ColorSpace::UNSPECIFIED);
  EXPECT_EQ(videoStream.getPixelFormat().name, "yuv420p");
  EXPECT_EQ(videoStream.getExtradata().size(), 46);
  EXPECT_EQ(videoStream.getIndex(), 1);
}

TEST(FFmpegTest, DemuxPackets)
{
  const auto loadingResult = FFmpegLibrariesInterfaceBuilder().tryLoadingOfLibraries();
  EXPECT_TRUE(loadingResult) << "Error loading libraries";

  Demuxer demuxer(loadingResult.librariesInterface);
  const auto [openSuccessfull, openingLog] = demuxer.openFile(TEST_FILE_NAME);
  EXPECT_TRUE(openSuccessfull) << "Opening test file " << TEST_FILE_NAME << " failed.";

  auto formatContext = demuxer.getFormatContext();

  constexpr std::array<int, 25> expectedDataSizesVideo = {3827, 499, 90,  46, 29,  439, 82, 36, 40,
                                                          461,  56,  36,  25, 419, 54,  29, 23, 339,
                                                          50,   46,  287, 54, 30,  171, 141};
  constexpr std::array<int, 45> expectedDataSizesAudio = {
      343, 377, 258, 299, 304, 339, 316, 362, 353, 358, 385, 358, 348, 365, 358,
      364, 369, 385, 339, 377, 348, 368, 370, 367, 353, 348, 397, 391, 410, 337,
      372, 370, 372, 342, 431, 378, 370, 333, 355, 366, 355, 388, 412, 349, 7};

  int packetCountAudio = 0;
  int packetCountVideo = 0;
  std::cerr << "Packet sizes: ";
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

    if (packet.getStreamIndex() == 0)
      std::cerr << packet.getDataSize() << ", ";
  }

  EXPECT_EQ(packetCountAudio, 45);
  EXPECT_EQ(packetCountVideo, 25);
}
