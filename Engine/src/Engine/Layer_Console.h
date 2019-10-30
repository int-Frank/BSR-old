#ifndef EN_LAYER_CONSOLE_H
#define EN_LAYER_CONSOLE_H

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
#define ITEM(x) virtual void HandleMessage(MessageSub<MT_##x> *);
    MESSAGE_LIST;

    void Update(float);

  };
}

#endif