#pragma once

class ILed {
  public:
    virtual ~ILed() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};
