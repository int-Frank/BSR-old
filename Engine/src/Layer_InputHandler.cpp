//@group Layers

#include <cstring>

#include "Layer_InputHandler.h"
#include "Framework.h"
#include "core_Log.h"
#include "InputCodes.h"
#include "EngineMessages.h"
#include "MessageBus.h"

#include "IEventPoller.h"
#include "IMouseController.h"

namespace Engine
{
  Layer_InputHandler::Layer_InputHandler()
    : m_eventPoller(Framework::Instance()->GetEventPoller())
    , m_mouseController(Framework::Instance()->GetMouseController())
    , m_xMouseRotRate(1.0f)
    , m_yMouseRotRate(1.0f)
  {
    //TODO move these to OnAttach(). Also Undind on OnDetatch()
    Bind(IC_MOUSE_MOTION, Message_Input_MouseMove::GetStaticID(), new Message_GUI_MouseMove());
    Bind(IC_MOUSE_WHEEL_UP, Message_Input_MouseWheelUp::GetStaticID(), new Message_GUI_MouseWheelUp());
    Bind(IC_MOUSE_WHEEL_DOWN, Message_Input_MouseWheelDown::GetStaticID(), new Message_GUI_MouseWheelDown());
    Bind(IC_MOUSE_BUTTON_LEFT, Message_Input_MouseButtonDown::GetStaticID(), new Message_GUI_MouseButtonDown());
    Bind(IC_MOUSE_BUTTON_LEFT, Message_Input_MouseButtonUp::GetStaticID(), new Message_GUI_MouseButtonUp());
    Bind(IC_MOUSE_BUTTON_RIGHT, Message_Input_MouseButtonDown::GetStaticID(), new Message_GUI_MouseButtonDown());
    Bind(IC_MOUSE_BUTTON_RIGHT, Message_Input_MouseButtonUp::GetStaticID(), new Message_GUI_MouseButtonUp());

    Bind(IC_UNKNOWN, Message_Input_Text::GetStaticID(), new Message_GUI_Text());
  }

  Layer_InputHandler::~Layer_InputHandler()
  {

  }

  void Layer_InputHandler::HandleBinding(uint64_t a_key, Message const * a_pMsg)
  {
    auto it = m_bindings.find(a_key);
    if (it != m_bindings.end())
    {
      TRef<Message> pMsg = it->second->CloneAsTRef();
      MessageTranslator::Translate(pMsg.Get(), a_pMsg);
      POST(pMsg);
    }
  }

  void Layer_InputHandler::HandleMessage(Message* a_pMsg)
  {
    if (a_pMsg->GetCategory() != MC_Input)
      return;

    DISPATCH_MESSAGE(Message_Input_Text);
    DISPATCH_MESSAGE(Message_Input_KeyUp);
    DISPATCH_MESSAGE(Message_Input_KeyDown);
    DISPATCH_MESSAGE(Message_Input_MouseButtonUp);
    DISPATCH_MESSAGE(Message_Input_MouseButtonDown);
    DISPATCH_MESSAGE(Message_Input_MouseWheelUp);
    DISPATCH_MESSAGE(Message_Input_MouseWheelDown);
    DISPATCH_MESSAGE(Message_Input_MouseMove);
  }

  void Layer_InputHandler::HandleMessage(Message_Input_Text* a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_UNKNOWN, Message_Input_Text::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void  Layer_InputHandler::HandleMessage(Message_Input_KeyUp * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->keyCode, Message_Input_KeyUp::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void  Layer_InputHandler::HandleMessage(Message_Input_KeyDown * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->keyCode, Message_Input_KeyDown::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void  Layer_InputHandler::HandleMessage(Message_Input_MouseButtonUp * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->button, Message_Input_MouseButtonUp::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void  Layer_InputHandler::HandleMessage(Message_Input_MouseButtonDown * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->button, Message_Input_MouseButtonDown::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void  Layer_InputHandler::HandleMessage(Message_Input_MouseWheelUp * a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_MOUSE_WHEEL_UP, Message_Input_MouseWheelUp::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void  Layer_InputHandler::HandleMessage(Message_Input_MouseWheelDown * a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_MOUSE_WHEEL_DOWN, Message_Input_MouseWheelDown::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void  Layer_InputHandler::HandleMessage(Message_Input_MouseMove * a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_MOUSE_MOTION, Message_Input_MouseMove::GetStaticID());
    HandleBinding(mapKey, a_pMsg);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_InputHandler::GrabMouse()
  {
    m_mouseController->Grab();
  }

  void Layer_InputHandler::ReleaseMouse()
  {
    m_mouseController->Release();
  }

  void Layer_InputHandler::SetMouseLookRate(float a_xRate, float a_yRate)
  {
    m_xMouseRotRate = a_xRate;
    m_yMouseRotRate = a_yRate;
  }

  uint64_t Layer_InputHandler::PackKey(uint32_t a_inputCode, uint32_t a_messageType)
  {
    return (uint64_t(a_inputCode) << 32) | a_messageType;
  }

  void Layer_InputHandler::ClearBindings()
  {
    m_bindings.clear();
  }

  void Layer_InputHandler::Bind(InputCode a_inputCode, uint32_t a_event, Message * a_binding)
  {
    m_bindings.insert(PackKey(a_inputCode, a_event), Ref<Message>(a_binding));
  }

  /*template<>
  void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_Menu>()
  {
    m_bindings.clear();

    Bind(IC_MOUSE_MOTION, Message_Input_OtherMouseEvent, Message_GUI_MouseMove);
    Bind(IC_MOUSE_WHEEL_UP, Message_Input_OtherMouseEvent, Message_GUI_MouseWheelUp);
    Bind(IC_MOUSE_WHEEL_DOWN, Message_Input_OtherMouseEvent, Message_GUI_MouseWheelDown);
    Bind(IC_MOUSE_BUTTON_LEFT, Message_Input_ButtonDown, Message_GUI_MouseButtonDown);
    Bind(IC_MOUSE_BUTTON_LEFT, Message_Input_ButtonUp, Message_GUI_MouseButtonUp);

    Bind(IC_UNKNOWN, Message_Input_Text, Message_GUI_Text);

    BindKeyDown(IC_KEY_TAB, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_LEFT, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_RIGHT, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_UP, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_DOWN, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_PAGEUP, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_PAGEDOWN, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_HOME, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_END, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_INSERT, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_DELETE, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_BACKSPACE, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_SPACE, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_ENTER, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_KPENTER, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_A, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_C, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_V, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_X, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_Y, true, Message_GUI_KeyDown);
    BindKeyDown(IC_KEY_Z, true, Message_GUI_KeyDown);

    Bind(IC_KEY_TAB, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_LEFT, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_RIGHT, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_UP, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_DOWN, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_PAGEUP, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_PAGEDOWN, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_HOME, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_END, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_INSERT, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_DELETE, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_BACKSPACE, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_SPACE, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_ENTER, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_KPENTER, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_A, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_C, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_V, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_X, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_Y, Message_Input_KeyUp, Message_GUI_KeyUp);
    Bind(IC_KEY_Z, Message_Input_KeyUp, Message_GUI_KeyUp);

    Bind(IC_KEY_ESC, Message_Input_KeyDown, Message_GoBack);

    m_mouseController->Release();
  }*/

  void Layer_InputHandler::Update(float a_dt)
  {
    while (true)
    {
      TRef<Message> pMsg = m_eventPoller->NextEvent();
      if (pMsg.Get() == nullptr)
        break;

      HandleMessage(&*pMsg);
      if (!pMsg->Is(Message::Flag::Handled))
        POST(pMsg);
    }
  }
}
