#ifndef EN_LAYER_H
#define EN_LAYER_H

#include <stdint.h>
#include "Message.h"
#include "MessageHandler.h"

#define ASSIGN_ID static uint32_t GetID() {return uint32_t(__COUNTER__);}

namespace Engine
{
  class MessageBus;

  //TODO Subscribers should collect messages and process them on a single call
  //     to Update(). This way, we can throw the subscriber on a separate thread
  //     and update.
  class Layer : public MessageHandler
  {
  public: 

    Layer(MessageBus * a_pMsgBus);
    virtual ~Layer();

    virtual void OnAttach(){}
    virtual void OnDetach(){}

    virtual void Update(float dt) =0;
    virtual void Render() {}
    virtual void DoImGui(){}

    void Post(Message *);

    Layer(Layer const &);
    Layer & operator=(Layer const &);

    MessageBus * m_pMsgBus;
  };
}

#endif