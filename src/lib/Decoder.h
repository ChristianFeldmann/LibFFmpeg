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

  enum class PushResult
  {
    Ok,
    NotPushedPullFramesFirst,
    Error
  };
  PushResult                            pushPacket(const avcodec::AVPacketWrapper &packet);
  void                                  setFlushing();
  std::optional<avutil::AVFrameWrapper> decodeNextFrame();

private:
  std::shared_ptr<IFFmpegLibraries>             libraries;
  std::optional<avcodec::AVCodecContextWrapper> decoderContext{};

  State decoderState{State::NotOpened};
  bool  flushing{};
};

} // namespace ffmpeg
