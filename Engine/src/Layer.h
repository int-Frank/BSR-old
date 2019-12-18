//@group Layers

#ifndef EN_LAYER_H
#define EN_LAYER_H

#include <stdint.h>
#include "MessageHandler.h"
#include "Memory.h"

#define ASSIGN_ID public:\
static ::Engine::Layer::ID GetStaticID() \
{\
  return reinterpret_cast<::Engine::Layer::ID>(GetStaticID);\
}\
::Engine::Layer::ID GetID() override {return GetStaticID();}

namespace Engine
{
  class MessageBus;

  //TODO Subscribers should collect messages and process them on a single call
  //     to Update(). This way, we can throw the subscriber on a separate thread
  //     and update.
  class Layer : public MessageHandler
  {
  public: 

    typedef intptr_t ID;

    virtual ~Layer(){}

    virtual ID GetID() {return 0;}

    virtual void OnAttach(){}
    virtual void OnDetach(){}

    virtual void Update(float dt) =0;
    virtual void Render() {}
    virtual void DoImGui(){}

    //Layer(Layer const &);
    //Layer & operator=(Layer const &);
  };
}

#endif