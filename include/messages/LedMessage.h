#pragma once
#include "LedBluetoothCommand.h"

struct LedMessage {
  uint32_t sequenceNumber;
  LedBluetoothCommand bluetoothCommand;
};
