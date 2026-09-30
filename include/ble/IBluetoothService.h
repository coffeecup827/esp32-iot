#pragma once
#include "NimBLEService.h"

class IBluetoothService {
  public:
    virtual ~IBluetoothService() = default;
    virtual bool registerWith(NimBLEService* service) = 0;
};
