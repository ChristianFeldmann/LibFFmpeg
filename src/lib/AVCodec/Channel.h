/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "common/EnumMapper.h"

namespace libffmpeg::avcodec
{

enum class Channel
{
  Unused,
  FrontLeft,
  FrontRight,
  FrontCenter,
  LowFrequency,
  BackLeft,
  BackRight,
  FrontLeftOfCenter,
  FrontRightOfCenter,
  BackCenter,
  SideLeft,
  SideRight,
  TopCenter,
  TopFrontLeft,
  TopFrontCenter,
  TopFrontRight,
  TopBackLeft,
  TopBackCenter,
  TopBackRight,
  StereoLeft,
  StereoRight,
  WideLeft,
  WideRight,
  SurroundDirectLeft,
  SurroundDirectRight,
  LowFrequency2,
  TopSideLeft,
  TopSideRight,
  BottomFrontCenter,
  BottomFrontLeft,
  BottomFrontRight,
  SideSurroundLeft,
  SideSurroundRight,
  TopSurroundLeft,
  TopSurroundRight,
  BinauralLeft,
  BinauralRight,
  Ambisonic
};

const EnumMapper<Channel>
    channelMapper({{Channel::FrontLeft, "FL", "Front Left"},
                   {Channel::FrontRight, "FR", "Front Right"},
                   {Channel::FrontCenter, "FC", "Front Center"},
                   {Channel::LowFrequency, "LFE", "Low Frequency"},
                   {Channel::BackLeft, "BL", "Back Left"},
                   {Channel::BackRight, "BR", "Back Right"},
                   {Channel::FrontLeftOfCenter, "FLC", "Front Left of Center"},
                   {Channel::FrontRightOfCenter, "FRC", "Front Right of Center"},
                   {Channel::BackCenter, "BC", "Back Center"},
                   {Channel::SideLeft, "SL", "Side Left"},
                   {Channel::SideRight, "SR", "Side Right"},
                   {Channel::TopCenter, "TC", "Top Center"},
                   {Channel::TopFrontLeft, "TFL", "Top Front Left"},
                   {Channel::TopFrontCenter, "TFC", "Top Front Center"},
                   {Channel::TopFrontRight, "TFR", "Top Front Right"},
                   {Channel::TopBackLeft, "TBL", "Top Back Left"},
                   {Channel::TopBackCenter, "TBC", "Top Back Center"},
                   {Channel::TopBackRight, "TBR", "Top Back Right"},
                   {Channel::StereoLeft, "SLT", "Stereo Left"},
                   {Channel::StereoRight, "SRT", "Stereo Right"},
                   {Channel::WideLeft, "WL", "Wide Left"},
                   {Channel::WideRight, "WR", "Wide Right"},
                   {Channel::SurroundDirectLeft, "SDL", "Surround Direct Left"},
                   {Channel::SurroundDirectRight, "SDR", "Surround Direct Right"},
                   {Channel::LowFrequency2, "LFE2", "Low Frequency 2"},
                   {Channel::TopSideLeft, "TSL", "Top Side Left"},
                   {Channel::TopSideRight, "TSR", "Top Side Right"},
                   {Channel::BottomFrontCenter, "BFC", "Bottom Front Center"},
                   {Channel::BottomFrontLeft, "BFL", "Bottom Front Left"},
                   {Channel::BottomFrontRight, "BFR", "Bottom Front Right"},
                   {Channel::SideSurroundLeft, "SSL", "Side Surround Left"},
                   {Channel::SideSurroundRight, "SSR", "Side Surround Right"},
                   {Channel::TopSurroundLeft, "TSL", "Top Surround Left"},
                   {Channel::TopSurroundRight, "TSR", "Top Surround Right"},
                   {Channel::BinauralLeft, "BL", "Binaural Left"},
                   {Channel::BinauralRight, "BR", "Binaural Right"},
                   {Channel::Ambisonic, "AMB", "Ambisonic"}});

struct ChannelInfo
{
  std::optional<Channel> channel;
  std::optional<int>     ambisonicIndex;
  std::string            name;

  bool operator==(const ChannelInfo &other) const
  {
    return this->channel == other.channel && this->ambisonicIndex == other.ambisonicIndex &&
           this->name == other.name;
  }
};

using ChannelLayout = std::vector<ChannelInfo>;

} // namespace libffmpeg::avcodec