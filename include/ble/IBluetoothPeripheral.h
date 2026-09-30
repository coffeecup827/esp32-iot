#pragma once
#include "BluetoothConnectionState.h"
#include <functional>

class IBluetoothPeripheral
{
  public:
    using ConnectionStateHandler = std::function<void(BluetoothConnectionState)>;

    virtual ~IBluetoothPeripheral() = default;
    virtual bool start() = 0;
    virtual bool isEncrypted() const = 0;
    virtual void notifyLedState(bool ledState) = 0;
    virtual void setConnectionStateHandler(ConnectionStateHandler handler) = 0;
};
