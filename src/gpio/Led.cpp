#include "led/Led.h"

Led::Led(gpio_num_t pin) : pin(pin), state(false) {

  gpio_config_t ledIOConfig {
    .pin_bit_mask = 1ULL << pin,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
  };

  gpio_config(&ledIOConfig);

  turnOff();
}

void Led::turnOff() {
  gpio_set_level(pin, 1);
  setState(false);
}

void Led::turnOn() {
  gpio_set_level(pin, 0);
  setState(true);
}

bool Led::isOn() const {
  return state;
}
