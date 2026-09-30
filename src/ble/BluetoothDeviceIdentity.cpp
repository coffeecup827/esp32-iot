#include "ble/BluetoothDeviceIdentity.h"
#include "esp_mac.h"
#include <algorithm>
#include "nvs.h"
#include "esp_log.h"

namespace {
  constexpr const char* TAG = "BluetoothDeviceIdentity";
}

BluetoothDeviceIdentity::BluetoothDeviceIdentity() :
 deviceId(deriveDeviceIdFromMac()), deviceName(loadDisplayNameFromNvs()) {
  if(deviceName.empty()) {
    if(saveDisplayNameToNvs("Light Control")) {
      ESP_LOGI(TAG, "Default DeviceName Set");
    } else {
      ESP_LOGE(TAG, "Default DeviceName Set");
    }
  }
}

std::string BluetoothDeviceIdentity::deriveDeviceIdFromMac() {
  uint8_t mac[6] = {0};
  esp_read_mac(mac, ESP_MAC_BT);
  char macAsString[13];
  std::snprintf(macAsString, sizeof(macAsString), "%02X%02X%02X%02X%02X%02X",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return std::string(macAsString);
};

bool BluetoothDeviceIdentity::isValidName(const std::string& name) {
  if (name.empty() || name.size() > MaxDeviceNameLength) return false;
  return std::all_of(name.begin(), name.end(), [](unsigned char c) {
    return c >= 0x20 && c < 0x7F;
  });
}

std::string BluetoothDeviceIdentity::loadDisplayNameFromNvs() const {
  nvs_handle storageHandle;

  if(nvs_open(NVSNameSpace, NVS_READONLY, &storageHandle) != ESP_OK) {
    ESP_LOGE(TAG, "nvs_open failed");
    return "";
  }

  char deviceNameCharArray[MaxDeviceNameLength + 1] = {0};

  size_t length = sizeof(deviceNameCharArray);

  esp_err_t result = nvs_get_str(storageHandle, NVSDeviceNameKey, deviceNameCharArray, &length);
  nvs_close(storageHandle);

  return result == ESP_OK ? std::string(deviceNameCharArray) : "";
}

bool BluetoothDeviceIdentity::saveDisplayNameToNvs(const std::string& deviceName) const {
  nvs_handle storageHandle;

  if(nvs_open(NVSNameSpace, NVS_READWRITE, &storageHandle) != ESP_OK) {
    ESP_LOGE(TAG, "nvs_open failed");
    return false;
  }

  esp_err_t result = nvs_set_str(storageHandle, NVSDeviceNameKey, deviceName.c_str());
  if(result == ESP_OK) {
    nvs_commit(storageHandle);
  }
  nvs_close(storageHandle);

  return result == ESP_OK;
}

const std::string& BluetoothDeviceIdentity::getDeviceId() const {
  return deviceId;
}

const std::string& BluetoothDeviceIdentity::getDeviceName() const {
  return deviceName;
}

bool BluetoothDeviceIdentity::setDeviceName(const std::string& name) {
  if(!isValidName(name)) {
    ESP_LOGW(TAG, "Device Name Invalid: %s", name);
    return false;
  }
  if(!saveDisplayNameToNvs(name)) {
    ESP_LOGW(TAG, "Device Rename Failed At NVS: %s", name);
    return false;
  }
  deviceName = name;
  return true;
}
