#pragma once
#include "Queue.h"
#include "../messages/LedMessage.h"
#include "ITask.h"
#include "../led/Led.h"

class LedControllerTask : public ITask {

  private:
    Queue<LedMessage>& ledCommandQueue;
    Led& ledDriver;

  protected:
    void run() override;

  public:
    LedControllerTask(Queue<LedMessage>& ledCommandQueue, Led& ledDriver);
};
