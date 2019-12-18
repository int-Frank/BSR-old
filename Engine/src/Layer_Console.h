//@group Layers

#ifndef EN_LAYER_CONSOLE_H
#define EN_LAYER_CONSOLE_H

#include "Message.h"
#include "Layer.h"

namespace Engine
{
  class Layer_Console : public Layer
  {
    ASSIGN_ID
  public:

    Layer_Console();
    ~Layer_Console();

#undef ITEM
#define ITEM(x) virtual void HandleMessage(MessageSub<MT_##x> *);
    MESSAGE_LIST;

    void Update(float);

  };
}

#endif