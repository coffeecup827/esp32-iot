#pragma once

class ILed {
  public:
    virtual ~ILed() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void setState() = 0;
    virtual bool isOn() const = 0;
};
