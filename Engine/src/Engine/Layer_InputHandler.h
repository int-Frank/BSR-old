#ifndef LAYER_INPUTHANDLER_H
#define LAYER_INPUTHANDLER_H

#include <stdint.h>

#include "DgMap_AVL.h"

#include "MessageBus.h"
#include "Message.h"
#include "Layer.h"
#include "InputCodes.h"

namespace Engine
{
  class IEventPoller;
  class IMouseController;

  class Layer_InputHandler : public Layer
  {
  public:

    ASSIGN_ID

    Layer_InputHandler(MessageBus *);
    ~Layer_InputHandler();

    void ClearBindings();
    void Update(float);

    void GrabMouse();
    void ReleaseMouse();
    void SetMouseLookRate(float xRate, float yRate);

    void HandleMessage(MessageSub<MT_Input_Text>*);
    void HandleMessage(MessageSub<MT_Input_KeyUp>*);
    void HandleMessage(MessageSub<MT_Input_KeyDown>*);
    void HandleMessage(MessageSub<MT_Input_MouseButtonUp>*);
    void HandleMessage(MessageSub<MT_Input_MouseButtonDown>*);
    void HandleMessage(MessageSub<MT_Input_MouseWheelUp>*);
    void HandleMessage(MessageSub<MT_Input_MouseWheelDown>*);
    void HandleMessage(MessageSub<MT_Input_MouseMove>*);

    void Bind(InputCode inputCode, MessageType event, Message * binding);

  private:

    uint64_t PackKey(uint32_t inputCode, MessageType MessageType);

    std::shared_ptr<IEventPoller>                   m_eventPoller;
    std::shared_ptr<IMouseController>               m_mouseController;

    float                                           m_xMouseRotRate;
    float                                           m_yMouseRotRate;
    Dg::Map_AVL<uint64_t, std::shared_ptr<Message>> m_bindings;

  };
}


#endif