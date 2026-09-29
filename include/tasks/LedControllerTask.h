#pragma once
#include "Queue.h"
#include "../messages/Message.h"
#include "ITask.h"
#include "../led/Led.h"

class LedControllerTask : public ITask {

  private:
    Queue<LedMessage>& inQueue;
    Led ledDriver;

  protected:
    void run() override;

  public:
    LedControllerTask(Queue<LedMessage>& outQueue, Led ledDriver);
};
