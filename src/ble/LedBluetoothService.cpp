#include "ble/LedBluetoothService.h"
#include "esp_log.h"

namespace {
  constexpr const char* TAG = "LedBluetoothService";
  constexpr const char* LED_COMMAND_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a5";
  constexpr const char* LED_STATUS_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a6";
}

LedBluetoothService::LedBluetoothService(LedCommandDispatcher& ledCommandDispatcher)
  : ledCommandDispatcher(ledCommandDispatcher) {
}

bool LedBluetoothService::registerWith(NimBLEService* bleService) {
  ledCommandCharacteristic = bleService->createCharacteristic(
    LED_COMMAND_UUID, 
    NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_ENC
  );
  ledStatusCharacteristic = bleService->createCharacteristic(
    LED_STATUS_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::NOTIFY
  );

  if(!ledCommandCharacteristic || !ledStatusCharacteristic) {
    return false;
  }

  ledCommandCharacteristic->setCallbacks(this);
  ledStatusCharacteristic->setValue(static_cast<uint8_t>(0));

  return true;
}

void LedBluetoothService::onLedStateChanged(bool ledState) {
  if(!ledStatusCharacteristic) {
    return;
  }
  ledStatusCharacteristic->setValue(static_cast<uint8_t>(ledState ? 1 : 0));
  ledStatusCharacteristic->notify();
}

void LedBluetoothService::onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo&) {
  if(characteristic == ledCommandCharacteristic) {
    const std::string bleInput = characteristic->getValue();

    if(bleInput.size() != 1) {
      ESP_LOGE(TAG, "Invalid LED BLE Command");
      return;
    }

    LedBluetoothCommand ledCommand {};
    switch (static_cast<uint8_t>(bleInput[0])) {
      case 0x00: ledCommand = LedBluetoothCommand::TURN_OFF; break;
      case 0x01: ledCommand = LedBluetoothCommand::TURN_ON;  break;
      default: return;
    }

    if(!ledCommandDispatcher.dispatch(ledCommand)) {
      ESP_LOGE(TAG, "Failed To Dispatch LED BLE Command");
      return;
    }
  }
}
