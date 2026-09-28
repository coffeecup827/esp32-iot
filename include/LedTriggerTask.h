#pragma once
#include "Queue.h"
#include "Message.h"
#include "ITask.h"

class LedTriggerTask : public ITask {

  private:
    Queue<LedMessage>& outQueue;
    const TickType_t periodTicks;
    uint32_t sequenceNumber_ = 0;

  protected:
    void run() override;

  public:
    LedTriggerTask(Queue<LedMessage>& outQueue, TickType_t perioidTicks);
};
