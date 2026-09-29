#include "tasks/LedConsumerTask.h"
#include "esp_log.h"

namespace {
  constexpr const char* TAG = "LedConsumer";
}

LedConsumerTask::LedConsumerTask(Queue<LedMessage>& ledCommandQueue, Led& ledDriver) 
  : ledCommandQueue(ledCommandQueue), ledDriver(ledDriver) {
    //empty constructor
  };

void LedConsumerTask::addObserver(ILedStateObserver* observer) {
  observers.push_back(observer);
}

void LedConsumerTask::run() {
    LedMessage msg{};
    while (true) {
        if (ledCommandQueue.receive(msg, portMAX_DELAY)) {
          if(msg.bluetoothCommand == LedBluetoothCommand::TURN_ON) {
            ledDriver.turnOn();
          } else {
            ledDriver.turnOff();
          }

          for(ILedStateObserver* observer: observers) {
            observer->onLedStateChanged(ledDriver.isOn());
          }

          ESP_LOGI(TAG, "Toggled LED for message #%lu", 
            static_cast<unsigned long>(msg.sequenceNumber));
        }
    }
}
