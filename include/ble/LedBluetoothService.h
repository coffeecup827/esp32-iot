#pragma once
#include "IBluetoothService.h"
#include "NimBLEService.h"
#include "tasks/ILedStateObserver.h"
#include "dispatcher/IDispatcher.h"
#include "messages/LedBluetoothCommand.h"

class LedBluetoothService : public IBluetoothService,
  public NimBLECharacteristicCallbacks,
  public ILedStateObserver
{
  private:
    IDispatcher<LedBluetoothCommand>& ledCommandDispatcher;
    NimBLECharacteristic* ledCommandCharacteristic {nullptr};
    NimBLECharacteristic* ledStatusCharacteristic {nullptr};

  public:
    LedBluetoothService(IDispatcher<LedBluetoothCommand>& ledCommandDispatcher);
    void onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& bleConnectionInfo) override;
    void onLedStateChanged(bool ledState) override;
    bool registerWith(NimBLEService* service) override;
};
