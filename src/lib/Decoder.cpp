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

  bool openContextSuccessfull = false;
  if (auto codecParameters = stream.getCodecParameters())
  {
    this->decoderContext   = avcodec::AVCodecContextWrapper(this->libraries);
    openContextSuccessfull = this->decoderContext->openContextForDecoding(*codecParameters);
  }
  else
  {
    this->decoderContext = stream.getCodecContext();
    if (!this->decoderContext)
      return false;
    openContextSuccessfull = this->decoderContext->openContextForDecoding();
  }

  this->decoderState = openContextSuccessfull ? State::NeedsMoreData : State::Error;
  return openContextSuccessfull;
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

  ReturnCode returnCode;
  if (this->libraries->getLibrariesVersion().avcodec.major == 56)
  {
    // In the old interface there is no pushPacket/pullFrame so we have
    // to adapt by decoding and storing the frames here.
    auto decodeResult = this->decoderContext->decodeVideo2(packet);
    returnCode        = decodeResult.returnCode;

    if (decodeResult.frame)
    {
      this->pendingDecodedFrame = std::move(decodeResult.frame);
      this->decoderState        = State::RetrieveFrames;
    }
  }
  else
    returnCode = this->decoderContext->sendPacket(packet);

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

  auto returnCode = ReturnCode::Ok;
  if (this->libraries->getLibrariesVersion().avcodec.major > 56)
    returnCode = this->decoderContext->sendFlushPacket();

  this->flushing     = true;
  this->decoderState = (returnCode == ReturnCode::Ok) ? State::RetrieveFrames : State::Error;
}

std::optional<avutil::AVFrameWrapper> Decoder::decodeNextFrame()
{
  if (this->decoderState != State::RetrieveFrames)
    return {};

  if (this->libraries->getLibrariesVersion().avcodec.major == 56)
  {
    if (this->flushing)
    {
      avcodec::AVPacketWrapper emptyFlushPacket(this->libraries);

      const auto dataPtr  = emptyFlushPacket.getData();
      const auto dataSize = emptyFlushPacket.getDataSize();

      auto decodeResult = this->decoderContext->decodeVideo2(emptyFlushPacket);
      if (decodeResult.frame)
        return std::move(decodeResult.frame);
      this->decoderState = State::EndOfBitstream;
      return {};
    }

    this->decoderState = State::NeedsMoreData;
    if (this->pendingDecodedFrame)
    {
      auto frame = std::move(this->pendingDecodedFrame);
      this->pendingDecodedFrame.reset();
      return frame;
    }
  }
  else
  {
    auto [frame, returnCode] = this->decoderContext->revieveFrame();

    if (returnCode == ReturnCode::Ok)
      return std::move(frame);

    if (returnCode == ReturnCode::TryAgain)
      this->decoderState = this->flushing ? State::EndOfBitstream : State::NeedsMoreData;
    else if (returnCode == ReturnCode::EndOfFile)
      this->decoderState = State::EndOfBitstream;
    else
      this->decoderState = State::Error;
  }

  return {};
}

} // namespace ffmpeg
