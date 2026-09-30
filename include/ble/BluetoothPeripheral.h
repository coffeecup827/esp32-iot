//have to decouple bluetooth and led separately i.e separate bluetooth connection and services.
#pragma once
#include "IBluetoothDeviceIdentity.h"
#include "dispatcher/LedCommandDispatcher.h"
#include "NimBLEDevice.h"
#include "IBluetoothPeripheral.h"
#include "tasks/ILedStateObserver.h"
#include "IBluetoothService.h"

class BluetoothPeripheral : public IBluetoothPeripheral,
  public NimBLEServerCallbacks,
  public NimBLECharacteristicCallbacks
{
  private:
    IBluetoothDeviceIdentity& deviceIdentity;
    
    NimBLEServer* bleServer {nullptr};
    NimBLECharacteristic* deviceNameCharacteristic {nullptr};
    NimBLECharacteristic* deviceIdCharacteristic {nullptr};
    std::vector<IBluetoothService*> bleServices;

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
    BluetoothPeripheral(IBluetoothDeviceIdentity& deviceIdentity, std::vector<IBluetoothService*> bleServices);

    bool start() override;
    bool isEncrypted() const override;
    void setConnectionStateHandler(ConnectionStateHandler handler) override;
};
