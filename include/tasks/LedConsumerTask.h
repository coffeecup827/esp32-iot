#pragma once
#include "Queue.h"
#include "../messages/LedMessage.h"
#include "ITask.h"
#include "../led/Led.h"

class LedConsumerTask : public ITask {

  private:
    Queue<LedMessage>& ledCommandQueue;
    Led& ledDriver;

  protected:
    void run() override;

  public:
    LedConsumerTask(Queue<LedMessage>& ledCommandQueue, Led& ledDriver);
};
