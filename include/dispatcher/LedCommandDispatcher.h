#pragma once
#include "cstdint"
#include "IDispatcher.h"
#include "messages/LedMessage.h"
#include "../tasks/Queue.h"

class LedCommandDispatcher : public IDispatcher<LedBluetoothCommand> {

  private:
    std::uint32_t sequenceNumber;
    Queue<LedMessage>& ledCommandQueue;

  public:
    LedCommandDispatcher(Queue<LedMessage>& ledCmmandQueue);
    bool dispatch(LedBluetoothCommand& command) override;

};

