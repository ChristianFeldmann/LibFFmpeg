/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVCodecContextWrapper.h>
#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <AVFormat/wrappers/AVStreamWrapper.h>
#include <AVUtil/wrappers/AVFrameWrapper.h>
#include <libHandling/IFFmpegLibraries.h>

namespace ffmpeg
{

class Decoder
{
public:
  Decoder(std::shared_ptr<IFFmpegLibraries> libraries);

  explicit operator bool() const;

  bool openForDecoding(const avformat::AVStreamWrapper &stream);

  enum class State
  {
    NotOpened,
    NeedsMoreData,
    RetrieveFrames,
    EndOfBitstream,
    Error
  };
  State getDecoderState() const { return this->decoderState; }

  enum class SendPacketResult
  {
    Ok,
    NotSentPullFramesFirst,
    Error
  };

  SendPacketResult                      sendPacket(const avcodec::AVPacketWrapper &packet);
  void                                  setFlushing();
  std::optional<avutil::AVFrameWrapper> decodeNextFrame();

private:
  std::shared_ptr<IFFmpegLibraries>             ffmpegLibraries;
  std::optional<avcodec::AVCodecContextWrapper> decoderContext{};

  // For the old (FFmpeg 2) interface, we store the frame that is returned
  // in sendPacket from avcodec_decode_video2.
  std::optional<avutil::AVFrameWrapper> pendingDecodedFrame;

  State decoderState{State::NotOpened};
  bool  flushing{};
};

} // namespace ffmpeg
