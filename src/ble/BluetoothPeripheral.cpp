#include "ble/BluetoothPeripheral.h"
#include "esp_log.h"
#include <cstdio>

namespace {
  constexpr const char* TAG = "BluetoothPeripheral";
  constexpr const char* SERVICE_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a4"; //correct
  constexpr const char* DEVICE_ID_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a7";
  constexpr const char* DEVICE_NAME_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a8";
}

BluetoothPeripheral::BluetoothPeripheral(IBluetoothDeviceIdentity& deviceIdentity, std::vector<IBluetoothService*> bleServices)
  : deviceIdentity(deviceIdentity), bleServices(bleServices) {
}

void BluetoothPeripheral::setState(BluetoothConnectionState state) {
  if(stateHandler) {
    stateHandler(state);
  }
}

bool BluetoothPeripheral::configureAdvertising() {
  NimBLEAdvertising* advertiser = NimBLEDevice::getAdvertising();
  advertiser->addServiceUUID(SERVICE_UUID);

  std::string deviceId = deviceIdentity.getDeviceId();
  std::string manufacturerData;

  manufacturerData.push_back(static_cast<char>(0xFF));
  manufacturerData.push_back(static_cast<char>(0xFF));

  for (int i = 0; i < 6; i++) {
    unsigned int b = 0;
    std::sscanf(deviceId.c_str() + i * 2, "%2x", &b);
    manufacturerData.push_back(static_cast<char>(b));
  }
  advertiser->setManufacturerData(manufacturerData);

  if(!advertiser->start()) return false;

  setState(BluetoothConnectionState::ADVERTISING);
  return true;
}

bool BluetoothPeripheral::start() {
  NimBLEDevice::init("");
  NimBLEDevice::setSecurityAuth(true, false, true);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);

  bleServer = NimBLEDevice::createServer();
  if(!bleServer) return false;
  bleServer->setCallbacks(this);

  NimBLEService* bleService = bleServer->createService(SERVICE_UUID);
  if(!bleService) return false;

  deviceNameCharacteristic = bleService->createCharacteristic(
    DEVICE_NAME_UUID,
    NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_ENC | NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC
  );
  deviceIdCharacteristic = bleService->createCharacteristic(
    DEVICE_ID_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC
  );

  for(IBluetoothService* bluetoothService : bleServices) {
    bool result = bluetoothService->registerWith(bleService);
    if(!result) {
      ESP_LOGE(TAG, "Failed to Register Service");
      return false;
    }
  }

  if(!deviceNameCharacteristic || !deviceIdCharacteristic) {
    return false;
  }

  deviceNameCharacteristic->setCallbacks(this);

  deviceNameCharacteristic->setValue(deviceIdentity.getDeviceName());
  deviceIdCharacteristic->setValue(deviceIdentity.getDeviceId());

  return configureAdvertising();
}

void BluetoothPeripheral::onConnect(NimBLEServer*, NimBLEConnInfo& bleConnectionInfo) {
  setState(BluetoothConnectionState::CONNECTED_UNENCRYPTED);
  NimBLEDevice::startSecurity(bleConnectionInfo.getConnHandle());
}

void BluetoothPeripheral::onAuthenticationComplete(NimBLEConnInfo& bleConnectionInfo) {
  isConnectionEncrypted = bleConnectionInfo.isEncrypted();
  
  if(isEncrypted()) {
    setState(BluetoothConnectionState::ENCRYPTED);
  } else {
    bleServer->disconnect(bleConnectionInfo.getConnHandle());
  }
}

void BluetoothPeripheral::onDisconnect(NimBLEServer* bleServer, NimBLEConnInfo& bleConnectionInfo, int reason){
  isConnectionEncrypted = false;

  setState(BluetoothConnectionState::DISCONNECTED);

  ESP_LOGI(TAG, "Disconnected reason=%d", reason);
  if (!configureAdvertising()) {
    ESP_LOGE(TAG, "Failed to resume advertising");
  }
}

void BluetoothPeripheral::onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo&) {
  if(characteristic == deviceNameCharacteristic) {
    const std::string newName = characteristic->getValue();
    if (deviceIdentity.setDeviceName(newName)) {
      deviceNameCharacteristic->setValue(deviceIdentity.getDeviceName());
    } else {
      ESP_LOGE(TAG, "BLE Device Rename Failed");
    }
  }
}

void BluetoothPeripheral::onRead(NimBLECharacteristic* characteristic, NimBLEConnInfo&) {

}

bool BluetoothPeripheral::isEncrypted() const {
  return isConnectionEncrypted;
}

void BluetoothPeripheral::setConnectionStateHandler(ConnectionStateHandler handler) {
  stateHandler = std::move(handler);
}
