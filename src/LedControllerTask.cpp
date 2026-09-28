#include "LedControllerTask.h"
#include "esp_log.h"

namespace {
  constexpr const char* TAG = "LedController";
}

LedControllerTask::LedControllerTask(Queue<LedMessage>& inQueue, Led ledDriver) 
  : inQueue(inQueue), ledDriver(ledDriver) {
    //empty constructor
  };

void LedControllerTask::run() {
    LedMessage msg{};
    while (true) {
        if (inQueue.receive(msg, portMAX_DELAY)) {
            ledDriver.turnOn();
            vTaskDelay(pdMS_TO_TICKS(1000));
            ledDriver.turnOff();
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI(TAG, "Toggled LED for message #%lu",
                     static_cast<unsigned long>(msg.sequenceNumber));
        }
    }
}
