#include "ble/BluetoothPeripheral.h"
#include "esp_log.h"
#include <cstdio>

namespace {
  constexpr const char* TAG = "BluetoothPeripheral";
  constexpr const char* SERVICE_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a4"; //correct
  constexpr const char* DEVICE_ID_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a7";
  constexpr const char* DEVICE_NAME_UUID = "303e4d0a-820b-439b-9db4-83a1a63bd0a8";
  constexpr const int MAX_BONDS = 5;
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
  if (!advertiser->addServiceUUID(SERVICE_UUID)) {
    ESP_LOGE(TAG, "Failed to add advertised service UUID");
    return false;
  }

  std::string deviceId = deviceIdentity.getDeviceId();
  std::string manufacturerData;

  manufacturerData.push_back(static_cast<char>(0xFF));
  manufacturerData.push_back(static_cast<char>(0xFF));

  for (int i = 0; i < 6; i++) {
    unsigned int b = 0;
    std::sscanf(deviceId.c_str() + i * 2, "%2x", &b);
    manufacturerData.push_back(static_cast<char>(b));
  }
  if (!advertiser->setManufacturerData(manufacturerData)) {
    ESP_LOGE(TAG, "Failed to set manufacturer data");
    return false;
  }

  return startAdvertising();
}

bool BluetoothPeripheral::startAdvertising() {
  if(!NimBLEDevice::getAdvertising()->start()) return false;
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

void BluetoothPeripheral::onConnect(NimBLEServer* server, NimBLEConnInfo& bleConnectionInfo) {
  if(server->getConnectedCount() > 1) {
    server->disconnect(bleConnectionInfo);
    return;
  }

  if(NimBLEDevice::getNumBonds() >= MAX_BONDS && !NimBLEDevice::isBonded(bleConnectionInfo.getIdAddress())) {
    NimBLEDevice::deleteBond(NimBLEDevice::getBondedAddress(0));
  }
  
  setState(BluetoothConnectionState::CONNECTED_UNENCRYPTED);
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
  if (!startAdvertising()) {
    ESP_LOGE(TAG, "Failed to resume advertising");
  }
}

void BluetoothPeripheral::onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo&) {
  if(characteristic == deviceNameCharacteristic) {
    const std::string newName = characteristic->getValue();
    if (!deviceIdentity.setDeviceName(newName)) {
      ESP_LOGE(TAG, "BLE Device Rename Failed");
    }
    // Always republish the stored name: NimBLE keeps the written value, so without this a
    // rejected name would be readable by the client as if it had been accepted.
    deviceNameCharacteristic->setValue(deviceIdentity.getDeviceName());
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
