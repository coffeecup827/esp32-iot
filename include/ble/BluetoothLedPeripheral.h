//have to decouple bluetooth and led separately i.e separate bluetooth connection and services.
#pragma once
#include "IBluetoothDeviceIdentity.h"
#include "dispatcher/LedCommandDispatcher.h"
#include "NimBLEDevice.h"
#include "IBluetoothPeripheral.h"
#include "tasks/ILedStateObserver.h"

class BluetoothLedPeripheral : public IBluetoothPeripheral,
  public NimBLEServerCallbacks,
  public NimBLECharacteristicCallbacks,
  public ILedStateObserver
{
  private:
    IBluetoothDeviceIdentity& deviceIdentity;
    LedCommandDispatcher& ledCommandDispatcher;
    
    NimBLEServer* bleServer {nullptr};
    NimBLECharacteristic* ledCommandCharacteristic {nullptr};
    NimBLECharacteristic* ledStatusCharacteristic {nullptr};
    NimBLECharacteristic* deviceNameCharacteristic {nullptr};
    NimBLECharacteristic* deviceIdCharacteristic {nullptr};

    ConnectionStateHandler stateHandler {};
    bool isConnectionEncrypted {false};

    void setState(BluetoothConnectionState state);
    bool configureAdvertising();

    void onConnect(NimBLEServer* bleServer, NimBLEConnInfo& bleConnectionInfo) override;
    void onAuthenticationComplete(NimBLEConnInfo& bleConnectionInfo) override;
    void onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& bleConnectionInfo) override;
    void onRead(NimBLECharacteristic* characteristic, NimBLEConnInfo& bleConnectionInfo) override;
    void onDisconnect(NimBLEServer* bleServer, NimBLEConnInfo& bleConnectionInfo, int reason) override;

  public:
    BluetoothLedPeripheral(IBluetoothDeviceIdentity& deviceIdentity, LedCommandDispatcher& ledCommandDispatcher);

    bool start() override;
    bool isEncrypted() const override;
    void notifyLedState(bool ledState) override;
    void onLedStateChanged(bool ledState) override;
    void setConnectionStateHandler(ConnectionStateHandler handler) override;
};
