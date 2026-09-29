#include "esp_log.h"
#include "led/Led.h"
#include "tasks/Queue.h"
#include "messages/LedMessage.h"
#include "tasks/LedConsumerTask.h"
#include "tasks/LedBlinkPublisherTask.h"

extern "C" {
  void app_main() {
    static Led onBoardLed(GPIO_NUM_8);

    static Queue<LedMessage> ledCommandQueue(5);

    if(!ledCommandQueue.isValid()) {
      ESP_LOGE("main", "Failed to create queue - halting startup");
      return;
    }

    static LedBlinkPublisherTask blinkPublisher(ledCommandQueue, pdMS_TO_TICKS(500));
    static LedConsumerTask ledConsumer(ledCommandQueue, onBoardLed);

    blinkPublisher.start("LedTrigger", 4096, tskIDLE_PRIORITY + 1);
    ledConsumer.start("LedController", 4096, tskIDLE_PRIORITY + 1);
  }
}
