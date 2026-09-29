#pragma once
#include "Queue.h"
#include "../messages/LedMessage.h"
#include "ITask.h"

class LedBlinkPublisherTask : public ITask {

  private:
    Queue<LedMessage>& ledCommandQueue;
    const TickType_t blinkDelay;
    uint32_t sequenceNumber;

  protected:
    void run() override;

  public:
    LedBlinkPublisherTask(Queue<LedMessage>& ledCommandQueue, TickType_t blinkDelay);
};
