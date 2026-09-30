#pragma once
#include "IBluetoothService.h"
#include "NimBLEService.h"
#include "tasks/ILedStateObserver.h"
#include "dispatcher/LedCommandDispatcher.h"

class LedBluetoothService : public IBluetoothService,
  public NimBLECharacteristicCallbacks,
  public ILedStateObserver
{
  private:
    LedCommandDispatcher& ledCommandDispatcher;
    NimBLECharacteristic* ledCommandCharacteristic {nullptr};
    NimBLECharacteristic* ledStatusCharacteristic {nullptr};

  public:
    LedBluetoothService(LedCommandDispatcher& ledCommandDispatcher);
    void onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& bleConnectionInfo) override;
    void onLedStateChanged(bool ledState) override;
    bool registerWith(NimBLEService* service) override;
};
