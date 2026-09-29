#include "tasks/LedControllerTask.h"
#include "esp_log.h"

namespace {
  constexpr const char* TAG = "LedController";
}

LedControllerTask::LedControllerTask(Queue<LedMessage>& ledCommandQueue, Led& ledDriver) 
  : ledCommandQueue(ledCommandQueue), ledDriver(ledDriver) {
    //empty constructor
  };

void LedControllerTask::run() {
    LedMessage msg{};
    while (true) {
        if (ledCommandQueue.receive(msg, portMAX_DELAY)) {
          if(msg.bluetoothCommand == LedBluetoothCommand::TURN_ON) {
            ledDriver.turnOn();
          } else {
            ledDriver.turnOff();
          }
          ESP_LOGI(TAG, "Toggled LED for message #%lu", 
            static_cast<unsigned long>(msg.sequenceNumber));
        }
    }
}
