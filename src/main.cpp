#include "esp_log.h"
#include "led/Led.h"
#include "tasks/Queue.h"
#include "messages/LedMessage.h"
#include "tasks/LedControllerTask.h"
#include "tasks/LedBlinkPublisherTask.h"

extern "C" {
  void app_main() {
    Led onBoardLed(GPIO_NUM_8);

    static Queue<LedMessage> ledCommandQueue(5);

    if(!ledCommandQueue.isValid()) {
      ESP_LOGE("main", "Failed to create queue - halting startup");
      return;
    }

    static LedBlinkPublisherTask trigger(ledCommandQueue, pdMS_TO_TICKS(500));
    static LedControllerTask controller(ledCommandQueue, onBoardLed);

    trigger.start("LedTrigger", 4096, tskIDLE_PRIORITY + 1);
    controller.start("LedController", 4096, tskIDLE_PRIORITY + 1);
  }
}
