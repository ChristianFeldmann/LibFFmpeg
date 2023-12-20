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

Decoder::SendPacketResult Decoder::sendPacket(const avcodec::AVPacketWrapper &packet)
{
  if (this->decoderState == State::NotOpened || this->decoderState == State::Error ||
      this->decoderState == State::EndOfBitstream || this->flushing)
    return SendPacketResult::Error;

  if (!packet)
    return SendPacketResult::Error;

  if (this->decoderState == State::RetrieveFrames)
    return SendPacketResult::NotSentPullFramesFirst;

  const auto returnCode = this->decoderContext->sendPacket(packet);

  if (returnCode == ReturnCode::TryAgain)
  {
    this->decoderState = State::RetrieveFrames;
    return SendPacketResult::NotSentPullFramesFirst;
  }
  if (returnCode != ReturnCode::Ok)
  {
    this->decoderState = State::Error;
    return SendPacketResult::Error;
  }

  return SendPacketResult::Ok;
}

void Decoder::setFlushing()
{
  if (this->flushing)
    throw std::runtime_error("Flushing was already set. Can not be set multiple times.");

  const auto returnCode = this->decoderContext->sendFlushPacket();

  this->flushing     = true;
  this->decoderState = (returnCode == ReturnCode::Ok) ? State::RetrieveFrames : State::Error;
}

std::optional<avutil::AVFrameWrapper> Decoder::decodeNextFrame()
{
  if (this->decoderState != State::RetrieveFrames)
    return {};

  auto [frame, returnCode] = this->decoderContext->revieveFrame();

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
