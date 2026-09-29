#include "tasks/LedTriggerTask.h"
#include "esp_log.h"

namespace {
  constexpr const char* TAG = "LedTrigger";
}

LedTriggerTask::LedTriggerTask(Queue<LedMessage>& outQueue, TickType_t periodTicks) 
  : outQueue(outQueue), periodTicks(periodTicks) {
    //empty constructor
  };

void LedTriggerTask::run() {
  while(true) {
    LedMessage message { 
      sequenceNumber_++ 
    };
    if(!outQueue.send(message, pdMS_TO_TICKS(100))) {
        ESP_LOGW(TAG, "Queue full, dropping toggle message #%lu", static_cast<unsigned long>(message.sequenceNumber));
    }
    vTaskDelay(periodTicks);
  };
}

