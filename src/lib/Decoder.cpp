/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Decoder.h"

#include <common/Error.h>

namespace ffmpeg
{

Decoder::Decoder(std::shared_ptr<IFFmpegLibraries> libraries)
{
  if (!libraries)
    throw std::runtime_error("Given libraries pointer is null");
  this->libraries = libraries;
}

Decoder::operator bool() const
{
  return this->decoderState != State::Error && this->decoderState != State::EndOfBitstream;
};

bool Decoder::openForDecoding(const avformat::AVStreamWrapper &stream)
{
  if (this->decoderState != State::NotOpened)
    throw std::runtime_error("Decoder was already opened.");

  const auto codecParameters = stream.getCodecParameters();
  if (!codecParameters)
  {
    this->decoderState = State::Error;
    return false;
  }

  if (auto context =
          avcodec::AVCodecContextWrapper::openContextForDecoding(*codecParameters, this->libraries))
    this->decoderContext = context.value();
  else
  {
    this->decoderState = State::Error;
    return false;
  }

  this->decoderState = State::NeedsMoreData;
  return true;
}

Decoder::PushResult Decoder::pushPacket(const avcodec::AVPacketWrapper &packet)
{
  if (this->decoderState == State::NotOpened || this->decoderState == State::Error ||
      this->decoderState == State::EndOfBitstream || this->flushing)
    return PushResult::Error;

  if (!packet)
    return PushResult::Error;

  if (this->decoderState == State::RetrieveFrames)
    return PushResult::NotPushedPullFramesFirst;

  const auto returnCode = toReturnCode(this->libraries->avcodec.avcodec_send_packet(
      this->decoderContext.getCodecContext(), packet.getPacket()));

  if (returnCode == ReturnCode::Ok)
    return PushResult::Ok;
  if (returnCode == ReturnCode::TryAgain)
  {
    this->decoderState = State::RetrieveFrames;
    return PushResult::NotPushedPullFramesFirst;
  }

  this->decoderState = State::Error;
  return PushResult::Error;
}

void Decoder::setFlushing()
{
  if (this->flushing)
    throw std::runtime_error("Flushing was already set. Can not be set multiple times.");

  const auto returnCode =
      this->libraries->avcodec.avcodec_send_packet(this->decoderContext.getCodecContext(), nullptr);

  this->flushing     = true;
  this->decoderState = (returnCode == 0) ? State::RetrieveFrames : State::Error;
}

std::optional<avutil::AVFrameWrapper> Decoder::decodeNextFrame()
{
  if (this->decoderState != State::RetrieveFrames)
    return {};

  avutil::AVFrameWrapper frame(this->libraries);

  const auto returnCode = toReturnCode(this->libraries->avcodec.avcodec_receive_frame(
      this->decoderContext.getCodecContext(), frame.getFrame()));

  const auto frameSize = frame.getSize();

  if (returnCode == ReturnCode::Ok)
    return std::move(frame);

  if (returnCode == ReturnCode::TryAgain)
    this->decoderState = this->flushing ? State::EndOfBitstream : State::NeedsMoreData;
  else if (returnCode == ReturnCode::EndOfFile)
    this->decoderState = State::EndOfBitstream;
  else
    this->decoderState = State::Error;

  return {};
}

} // namespace ffmpeg
