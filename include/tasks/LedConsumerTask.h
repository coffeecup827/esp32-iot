#pragma once
#include <vector>
#include "Queue.h"
#include "../messages/LedMessage.h"
#include "ITask.h"
#include "../led/Led.h"
#include "ILedStateObserver.h"

class LedConsumerTask : public ITask {

  private:
    Queue<LedMessage>& ledCommandQueue;
    Led& ledDriver;
    std::vector<ILedStateObserver*> observers;

  protected:
    void run() override;

  public:
    LedConsumerTask(Queue<LedMessage>& ledCommandQueue, Led& ledDriver);
    void addObserver(ILedStateObserver* observer);
};
