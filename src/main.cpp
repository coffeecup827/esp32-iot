#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

extern "C" {
  void app_main() {
    static const char* TAG = "main";

    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_LOGI(TAG, "Hello, World!");

    gpio_config_t io_conf = {
      .pin_bit_mask = 1ULL << GPIO_NUM_8,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    
    gpio_set_level(GPIO_NUM_8, 0);
    vTaskDelay(pdMS_TO_TICKS(2000));

    gpio_set_level(GPIO_NUM_8, 1);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
