#include "dispatcher/LedCommandDispatcher.h"

LedCommandDispatcher::LedCommandDispatcher(Queue<LedMessage>& queue)
  : ledCommandQueue(queue) {
};

bool LedCommandDispatcher::dispatch(LedBluetoothCommand& command) {

  LedMessage message {
    .sequenceNumber = sequenceNumber,
    .bluetoothCommand = command,
  };

  return ledCommandQueue.send(message, 0);
};
