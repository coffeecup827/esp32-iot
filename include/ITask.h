#pragma once
#include "freertos/FreeRTOS.h"

class ITask {

  private:
    static void taskEntryPoint(void* pvParameters) {
      static_cast<ITask*>(pvParameters)->run();
    }

  protected:
    virtual void run() = 0;

  public:
    virtual ~ITask() = default;

    void start(const char* name, uint32_t stackSize, UBaseType_t priority) {
      xTaskCreate(taskEntryPoint, name, stackSize, this, priority, nullptr);
    }
};