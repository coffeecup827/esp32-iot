#pragma once
#include "IBluetoothDeviceIdentity.h"

class BluetoothDeviceIdentity : public IBluetoothDeviceIdentity {

  private:
    static constexpr std::size_t MaxDeviceNameLength = 20;
    static constexpr const char* NVSNameSpace = "bluetooth";
    static constexpr const char* NVSDeviceNameKey = "device_name";

    std::string deviceId;
    std::string deviceName;

    static std::string deriveDeviceIdFromMac();
    static bool isValidName(const std::string& deviceName);
    std::string loadDisplayNameFromNvs() const;
    bool saveDisplayNameToNvs(const std::string& deviceName) const;

  public:
    BluetoothDeviceIdentity();
    const std::string& getDeviceId() const override;
    const std::string& getDeviceName() const override;
    bool setDeviceName(const std::string& name) override;
};
