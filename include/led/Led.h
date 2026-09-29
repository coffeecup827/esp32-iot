#pragma once
#include "ILed.h"
#include "driver/gpio.h"

class Led : public ILed {
  private:
    gpio_num_t pin;
    bool state;
  public:
    Led(gpio_num_t pin);
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    void setState(bool newState) {
      this->state = newState;
    }
};
