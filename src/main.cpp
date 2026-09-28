#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "Led.h"

extern "C" {
  void app_main() {
    Led onBoardLed(GPIO_NUM_8);
    static const char* TAG = "main";

    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_LOGI(TAG, "Hello, World!");

    onBoardLed.turnOn();
    vTaskDelay(pdMS_TO_TICKS(2000));

    onBoardLed.turnOff();
    vTaskDelay(pdMS_TO_TICKS(2000));

    onBoardLed.turnOn();
  }
}
