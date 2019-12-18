//@group Layers

#ifndef EN_LAYER_CONSOLE_H
#define EN_LAYER_CONSOLE_H

#include "Message.h"
#include "Layer.h"

namespace Engine
{
  class Layer_Console : public Layer
  {
  public:

    static uint32_t GetID()
    {
      return 2;
    }

    uint32_t GetThisID() override
    {
      return 2;
    }

    Layer_Console();
    ~Layer_Console();

#undef ITEM
#define ITEM(x) virtual void HandleMessage(MessageSub<MT_##x> *);
    MESSAGE_LIST;

    void Update(float);

  };
}

#endif