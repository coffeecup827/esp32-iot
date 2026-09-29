#pragma once
#include "freertos/FreeRTOS.h"

class ITask {

  private:
    // need explanation for this method
    static void taskEntryPoint(void* pvParameters) {
      static_cast<ITask*>(pvParameters)->run();
    }

  protected:
    virtual void run() = 0;

  public:
    virtual ~ITask() = default;

    bool start(const char* name, uint32_t stackSize, UBaseType_t priority) {
      BaseType_t result = xTaskCreate(taskEntryPoint, name, stackSize, this, priority, nullptr);
      return result == pdPASS;
    }
};