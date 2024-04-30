/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <common/InternalTypes.h>

using libffmpeg::internal::AVPixelFormat;

namespace libffmpeg::avutil
{

template <typename AVPixFmtDescriptorType>
AVPixFmtDescriptorType createRawFormatDescriptor(const PixelFormatDescriptor &pixelFormat);

}
