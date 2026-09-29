#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

template <typename T> class Queue {
  
  private:
    QueueHandle_t handle = nullptr;

  public:
    Queue(size_t length) {
      handle = xQueueCreate(length, sizeof(T));
    }

    ~Queue() {
      if(handle != nullptr) {
        vQueueDelete(handle);
      }
    }

    bool send(const T& item, TickType_t tickstoWait) { // conceptually appends to the queue
      return xQueueSend(handle, &item, tickstoWait) == pdTRUE;
    }

    bool receive(T& outItem, TickType_t ticksToWait) { // conceptually pooutItemsps the queue
      return xQueueReceive(handle, &outItem, ticksToWait) == pdTRUE;
    }

    [[nodiscard]]
    bool isValid() const {
      return handle != nullptr;
    }

    // make queue uncopyable
    // remove constructor copy
    Queue(const Queue&) = delete;
    // remove = operation
    Queue& operator=(const Queue&) = delete;
};
