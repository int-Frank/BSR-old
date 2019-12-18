//@group Layers

#ifndef EN_LAYER_H
#define EN_LAYER_H

#include <stdint.h>
#include "MessageHandler.h"
#include "Memory.h"

#define ASSIGN_ID public:\
static uint32_t GetID() {return uint32_t(__COUNTER__ + 1);}\
uint32_t GetThisID() override {return GetID();}

namespace Engine
{
  class MessageBus;

  //TODO Subscribers should collect messages and process them on a single call
  //     to Update(). This way, we can throw the subscriber on a separate thread
  //     and update.
  class Layer : public MessageHandler
  {
  public: 

    virtual ~Layer(){}

    virtual uint32_t GetThisID() {return 0;}

    virtual void OnAttach(){}
    virtual void OnDetach(){}

    virtual void Update(float dt) =0;
    virtual void Render() {}
    virtual void DoImGui(){}

    //Layer(Layer const &);
    //Layer & operator=(Layer const &);

    //MessageBus * m_pMsgBus;
  };
}

#endif