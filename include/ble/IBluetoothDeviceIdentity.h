#pragma once
#include <string>

class IBluetoothDeviceIdentity {
  public:
    virtual ~IBluetoothDeviceIdentity() = default;
    virtual const std::string& getDeviceId() const = 0;
    virtual const std::string& getDeviceName() const = 0;
    virtual bool setDeviceName(const std::string& deviceName) = 0;
};
