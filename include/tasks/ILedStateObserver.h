#pragma once

class ILedStateObserver {
  public:
    // make sure all the needed classes have destructors
    virtual ~ILedStateObserver() = default;
    virtual void onLedStateChanged(bool state) = 0;
};
