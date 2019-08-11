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

    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_Text>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_KeyUp>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_KeyDown>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_MouseButtonUp>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_MouseButtonDown>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_MouseWheelUp>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_MouseWheelDown>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Input_MouseMove>*);

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