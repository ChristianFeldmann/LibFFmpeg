/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVIOContextWrapper.h"

#include <common/Error.h>

namespace libffmpeg::avformat
{

using internal::AVIOContext;

namespace
{

static int read_packet_callback(void *opaque, uint8_t *buffer, int bufferSize)
{
  auto *io = reinterpret_cast<AVIOInputContext *>(opaque);

  if (const auto result = io->read_packet(buffer, bufferSize))
    return *result;

  return toAVError(ReturnCode::EndOfFile);
}

} // namespace

AVIOInputContext::AVIOInputContext(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
{
  const size_t bufferSize = 4096;
  auto buffer = static_cast<unsigned char *>(ffmpegLibraries->avutil.av_mallocz(bufferSize));
  if (buffer == nullptr)
  {
    ffmpegLibraries->log(LogLevel::Error, "Error allocating buffer for IO context.");
    return;
  }

  auto newContext = ffmpegLibraries->avformat.avio_alloc_context(
      buffer, bufferSize, 0, this, &read_packet_callback, nullptr, nullptr);

  this->ioContext = std::unique_ptr<AVIOContext, AVIOContextDeleter>(
      newContext, AVIOContextDeleter(ffmpegLibraries));
}

AVIOContext *AVIOInputContext::getAVIOContext() const
{
  return this->ioContext.get();
}

void AVIOInputContext::AVIOContextDeleter::operator()(AVIOContext *ioContext) const noexcept
{
  if (ioContext == nullptr)
    return;

  if (this->ffmpegLibraries->getLibrariesVersion().avformat.major > 56)
    this->ffmpegLibraries->avformat.avio_context_free(&ioContext);
  else
    this->ffmpegLibraries->avutil.av_freep(&ioContext);
}

} // namespace libffmpeg::avformat
