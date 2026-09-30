#include "tasks/LedBlinkPublisherTask.h"
#include "esp_log.h"

namespace {
  constexpr const char* TAG = "LedBlinkPublisher";
}

LedBlinkPublisherTask::LedBlinkPublisherTask(Queue<LedMessage>& queue, TickType_t blinkDelay) 
  : ledCommandQueue(queue), blinkDelay(blinkDelay) {
};

void LedBlinkPublisherTask::run() {
  while (true)
  {
    LedMessage message {
      .sequenceNumber = sequenceNumber++,
      .bluetoothCommand = (sequenceNumber%2) == 0 ? LedBluetoothCommand::TURN_OFF : LedBluetoothCommand::TURN_ON
    };

    if(!ledCommandQueue.send(message, 100)) {
      ESP_LOGW(TAG, "Queue full, drop #%lu", static_cast<unsigned long>(message.sequenceNumber));
    }

    vTaskDelay(blinkDelay);
  }
}
