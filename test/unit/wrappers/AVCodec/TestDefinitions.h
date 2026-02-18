/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVCodec/wrappers/AVChannelInternal.h>

namespace libffmpeg::avcodec
{

constexpr uint64_t TEST_CHANNEL_LAYOUT_STEREO      = 0b11;
constexpr uint64_t TEST_CHANNEL_LAYOUT_5POINT1     = 0b11000001111;
constexpr uint64_t TEST_CHANNEL_LAYOUT_7POINT1     = 0b11000111111;
constexpr uint64_t TEST_CHANNEL_LAYOUT_TERMINATION = 0;
constexpr uint64_t TEST_CHANNEL_FORMATS[4]         = {TEST_CHANNEL_LAYOUT_STEREO,
                                                      TEST_CHANNEL_LAYOUT_5POINT1,
                                                      TEST_CHANNEL_LAYOUT_7POINT1,
                                                      TEST_CHANNEL_LAYOUT_TERMINATION};

using internal::avcodec::AVChannelLayout;
using internal::avcodec::AVChannelOrder;
constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUT_STEREO(AVChannelOrder::AV_CHANNEL_ORDER_NATIVE,
                                                       2,
                                                       {TEST_CHANNEL_LAYOUT_STEREO});
constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUT_5POINT1(AVChannelOrder::AV_CHANNEL_ORDER_NATIVE,
                                                        2,
                                                        {TEST_CHANNEL_LAYOUT_5POINT1});
constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUT_7POINT1(AVChannelOrder::AV_CHANNEL_ORDER_NATIVE,
                                                        2,
                                                        {TEST_CHANNEL_LAYOUT_7POINT1});
constexpr AVChannelLayout
    TEST_AVCHANNEL_LAYOUT_TERMINATION(AVChannelOrder::AV_CHANNEL_ORDER_UNSPEC, 0, {0});
constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUTS[4] = {TEST_AVCHANNEL_LAYOUT_STEREO,
                                                       TEST_AVCHANNEL_LAYOUT_5POINT1,
                                                       TEST_AVCHANNEL_LAYOUT_7POINT1,
                                                       TEST_AVCHANNEL_LAYOUT_TERMINATION};

const std::initializer_list<ChannelInfo> TEST_CHANNELINFO_STEREO = {
    {Channel::FrontLeft, {}, "FL"}, {Channel::FrontRight, {}, "FR"}};

const std::initializer_list<ChannelInfo> TEST_CHANNELINFO_5POINT1 = {
    {Channel::FrontLeft, {}, "FL"},
    {Channel::FrontRight, {}, "FR"},
    {Channel::FrontCenter, {}, "FC"},
    {Channel::LowFrequency, {}, "LFE"},
    {Channel::SideLeft, {}, "SL"},
    {Channel::SideRight, {}, "SR"}};

const std::initializer_list<ChannelInfo> TEST_CHANNELINFO_7POINT1 = {
    {Channel::FrontLeft, {}, "FL"},
    {Channel::FrontRight, {}, "FR"},
    {Channel::FrontCenter, {}, "FC"},
    {Channel::LowFrequency, {}, "LFE"},
    {Channel::BackLeft, {}, "BL"},
    {Channel::BackRight, {}, "BR"},
    {Channel::SideLeft, {}, "SL"},
    {Channel::SideRight, {}, "SR"}};

} // namespace libffmpeg::avcodec
