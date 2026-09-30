#include "esp_log.h"
#include "led/Led.h"
#include "tasks/Queue.h"
#include "messages/LedMessage.h"
#include "tasks/LedConsumerTask.h"
#include "nvs_flash.h"
#include "dispatcher/LedCommandDispatcher.h"
#include "ble/BluetoothDeviceIdentity.h"
#include "ble/BluetoothPeripheral.h"
#include "ble/LedBluetoothService.h"

namespace {
  constexpr const char* TAG = "Main";
}

extern "C" {
  void app_main() {
    esp_err_t nvsInitResult = nvs_flash_init();
    if(nvsInitResult == ESP_ERR_NVS_NO_FREE_PAGES || nvsInitResult == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      nvsInitResult = nvs_flash_init();
    }
    ESP_ERROR_CHECK(nvsInitResult);

    static Led onBoardLed(GPIO_NUM_8);
    static Queue<LedMessage> ledCommandQueue(8);
    if(!ledCommandQueue.isValid()) {
      ESP_LOGE("main", "Failed to create queue - halting startup");
      return;
    }

    static LedConsumerTask ledConsumer(ledCommandQueue, onBoardLed);
    static LedCommandDispatcher ledCommandDispatcher(ledCommandQueue);

    // static LedBluetoothService ledBluetoothService(ledCommandDispatcher);

    static BluetoothDeviceIdentity bluetoothDeviceIdentity;
    static BluetoothPeripheral bluetoothPeripheral(bluetoothDeviceIdentity);

    // ledConsumer.addObserver(&ledBluetoothService);

    bluetoothPeripheral.setConnectionStateHandler([](BluetoothConnectionState state) {
      switch (state) {
          case BluetoothConnectionState::ADVERTISING: ESP_LOGI(TAG, "BLE Advertising"); break;
          case BluetoothConnectionState::CONNECTED_UNENCRYPTED: ESP_LOGI(TAG, "BLE Connected (unencrypted)"); break;
          case BluetoothConnectionState::ENCRYPTED: ESP_LOGI(TAG, "BLE Encrypted/Bonded"); break;
          case BluetoothConnectionState::DISCONNECTED: ESP_LOGI(TAG, "BLE Disconnected"); break;
        }
      }
    );

    ledConsumer.start("LedController", 4096, tskIDLE_PRIORITY + 1);

    if(!bluetoothPeripheral.start()) {
      ESP_LOGE(TAG, "BLE failed to start (continuing without BLE)");
    }

    ESP_LOGI(TAG, "BLE Ready: id=%s name=%s",
      bluetoothDeviceIdentity.getDeviceId().c_str(),
      bluetoothDeviceIdentity.getDeviceName().c_str()
    );
  }
}
