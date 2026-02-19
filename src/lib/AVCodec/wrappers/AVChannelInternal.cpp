/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVChannelInternal.h"

#include "../Channel.h"

#include <bitset>

namespace libffmpeg::internal::avcodec
{

using libffmpeg::avcodec::Channel;
using libffmpeg::avcodec::ChannelInfo;

namespace
{

ChannelLayout createUndefinedChannelLayout(const int numberChannels)
{
  ChannelLayout channelLayout;
  for (int ch = 0; ch < numberChannels; ++ch)
  {
    ChannelInfo channelInfo;
    channelInfo.name = "Unspecified";
    channelLayout.push_back(channelInfo);
  }
  return channelLayout;
}

std::optional<Channel> avChannelToChannel(const AVChannel avChannel)
{
  switch (avChannel)
  {
  case AVChannel::AV_CHAN_UNUSED:
    return Channel::Unused;
  case AVChannel::AV_CHAN_FRONT_LEFT:
    return Channel::FrontLeft;
  case AVChannel::AV_CHAN_FRONT_RIGHT:
    return Channel::FrontRight;
  case AVChannel::AV_CHAN_FRONT_CENTER:
    return Channel::FrontCenter;
  case AVChannel::AV_CHAN_LOW_FREQUENCY:
    return Channel::LowFrequency;
  case AVChannel::AV_CHAN_BACK_LEFT:
    return Channel::BackLeft;
  case AVChannel::AV_CHAN_BACK_RIGHT:
    return Channel::BackRight;
  case AVChannel::AV_CHAN_FRONT_LEFT_OF_CENTER:
    return Channel::FrontLeftOfCenter;
  case AVChannel::AV_CHAN_FRONT_RIGHT_OF_CENTER:
    return Channel::FrontRightOfCenter;
  case AVChannel::AV_CHAN_BACK_CENTER:
    return Channel::BackCenter;
  case AVChannel::AV_CHAN_SIDE_LEFT:
    return Channel::SideLeft;
  case AVChannel::AV_CHAN_SIDE_RIGHT:
    return Channel::SideRight;
  case AVChannel::AV_CHAN_TOP_CENTER:
    return Channel::TopCenter;
  case AVChannel::AV_CHAN_TOP_FRONT_LEFT:
    return Channel::TopFrontLeft;
  case AVChannel::AV_CHAN_TOP_FRONT_CENTER:
    return Channel::TopFrontCenter;
  case AVChannel::AV_CHAN_TOP_FRONT_RIGHT:
    return Channel::TopFrontRight;
  case AVChannel::AV_CHAN_TOP_BACK_LEFT:
    return Channel::TopBackLeft;
  case AVChannel::AV_CHAN_TOP_BACK_CENTER:
    return Channel::TopBackCenter;
  case AVChannel::AV_CHAN_TOP_BACK_RIGHT:
    return Channel::TopBackRight;
  case AVChannel::AV_CHAN_STEREO_LEFT:
    return Channel::StereoLeft;
  case AVChannel::AV_CHAN_STEREO_RIGHT:
    return Channel::StereoRight;
  case AVChannel::AV_CHAN_WIDE_LEFT:
    return Channel::WideLeft;
  case AVChannel::AV_CHAN_WIDE_RIGHT:
    return Channel::WideRight;
  case AVChannel::AV_CHAN_SURROUND_DIRECT_LEFT:
    return Channel::SurroundDirectLeft;
  case AVChannel::AV_CHAN_SURROUND_DIRECT_RIGHT:
    return Channel::SurroundDirectRight;
  case AVChannel::AV_CHAN_LOW_FREQUENCY_2:
    return Channel::LowFrequency2;
  case AVChannel::AV_CHAN_TOP_SIDE_LEFT:
    return Channel::TopSideLeft;
  case AVChannel::AV_CHAN_TOP_SIDE_RIGHT:
    return Channel::TopSideRight;
  case AVChannel::AV_CHAN_BOTTOM_FRONT_CENTER:
    return Channel::BottomFrontCenter;
  case AVChannel::AV_CHAN_BOTTOM_FRONT_LEFT:
    return Channel::BottomFrontLeft;
  case AVChannel::AV_CHAN_BOTTOM_FRONT_RIGHT:
    return Channel::BottomFrontRight;
  case AVChannel::AV_CHAN_SIDE_SURROUND_LEFT:
    return Channel::SideSurroundLeft;
  case AVChannel::AV_CHAN_SIDE_SURROUND_RIGHT:
    return Channel::SideSurroundRight;
  case AVChannel::AV_CHAN_TOP_SURROUND_LEFT:
    return Channel::TopSurroundLeft;
  case AVChannel::AV_CHAN_TOP_SURROUND_RIGHT:
    return Channel::TopSurroundRight;
  case AVChannel::AV_CHAN_BINAURAL_LEFT:
    return Channel::BinauralLeft;
  case AVChannel::AV_CHAN_BINAURAL_RIGHT:
    return Channel::BinauralRight;
  default:
    return {};
  }
}

ChannelLayout customChannelMapToLayout(const AVChannelCustom *map, int numberChannels)
{
  ChannelLayout layout;

  auto iterator = map;
  for (int i = 0; i < numberChannels; ++i)
  {
    ChannelInfo channelInfo;
    channelInfo.name = std::string(iterator->name);

    if (iterator->id == AVChannel::AV_CHAN_AMBISONIC_BASE)
    {
      channelInfo.ambisonicIndex =
          static_cast<int>(iterator->id) - static_cast<int>(AVChannel::AV_CHAN_AMBISONIC_BASE);
      channelInfo.channel = Channel::Ambisonic;
    }
    else
      channelInfo.channel = avChannelToChannel(iterator->id);

    layout.push_back(channelInfo);
    iterator++;
  }

  return layout;
}

} // namespace

ChannelLayout bitMaskToChannelLayout(const uint64_t mask, const bool isAmbisonic)
{
  ChannelLayout layout;

  constexpr auto MAX_CHANNELS = 64;

  std::bitset<MAX_CHANNELS> bits(mask);
  for (int bitPosition = 0; bitPosition < MAX_CHANNELS; ++bitPosition)
  {
    if (bits.test(bitPosition))
    {
      const auto avChannel = static_cast<AVChannel>(bitPosition);
      if (const auto channel = avChannelToChannel(avChannel))
      {
        ChannelInfo channelInfo;
        channelInfo.channel = *channel;
        channelInfo.name    = libffmpeg::avcodec::channelMapper.getName(*channel);
        if (isAmbisonic)
          channelInfo.ambisonicIndex = bitPosition;
        layout.push_back(channelInfo);
      }
    }
  }

  return layout;
}

std::vector<ChannelLayout> maskArrayToChannelLayouts(const uint64_t *masks)
{
  std::vector<ChannelLayout> layouts;
  int                        i    = 0;
  auto                       mask = masks[i];
  while (mask != 0)
  {
    layouts.push_back(bitMaskToChannelLayout(mask));

    i++;
    mask = masks[i];
  }
  return layouts;
}

ChannelLayout avChannelLayoutToChannelLayout(const AVChannelLayout &avLayout)
{
  if (avLayout.order == AVChannelOrder::AV_CHANNEL_ORDER_UNSPEC)
    return createUndefinedChannelLayout(avLayout.nb_channels);
  else if (avLayout.order == AVChannelOrder::AV_CHANNEL_ORDER_NATIVE)
    return bitMaskToChannelLayout(avLayout.u.mask);
  else if (avLayout.order == AVChannelOrder::AV_CHANNEL_ORDER_CUSTOM)
    return customChannelMapToLayout(avLayout.u.map, avLayout.nb_channels);
  else if (avLayout.order == AVChannelOrder::AV_CHANNEL_ORDER_AMBISONIC)
    return bitMaskToChannelLayout(avLayout.u.mask, true);

  throw std::runtime_error("Invalid channel layout order");
}

std::vector<ChannelLayout> avChannelLayoutListToChannelLayouts(const AVChannelLayout *layoutList)
{
  std::vector<ChannelLayout> layouts;

  int  i      = 0;
  auto layout = layoutList[i];
  while (layout.nb_channels != 0)
  {
    layouts.push_back(avChannelLayoutToChannelLayout(layout));
    i++;
    layout = layoutList[i];
  }

  return layouts;
}

} // namespace libffmpeg::internal::avcodec
