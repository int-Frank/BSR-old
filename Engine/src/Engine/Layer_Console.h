#ifndef LAYER_CONSOLE_H
#define LAYER_CONSOLE_H

#include "Message.h"
#include "Layer.h"

namespace Engine
{
  class Layer_Console : public Layer
  {
  public:

    ASSIGN_ID

    Layer_Console(MessageBus *);
    ~Layer_Console();

#undef ITEM
#define ITEM(x) virtual MessageHandlerReturnCode HandleMessage(MessageSub<MT_##x> *);
    MESSAGE_TYPES;

    void Update(float);

  };
}

#endif