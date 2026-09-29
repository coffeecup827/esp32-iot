#pragma once
#include "cstdint"

enum class LedBluetoothCommand : std::uint8_t {
  TURN_OFF = 0x00,
  TURN_ON = 0x01,
};
