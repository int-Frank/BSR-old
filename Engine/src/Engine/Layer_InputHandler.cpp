
#include <cstring>

#include "Layer_InputHandler.h"
#include "../Framework/Framework.h"
#include "Log.h"
#include "InputCodes.h"
#include "Message.h"
#include "MessageTranslator.h"

#include "IEventPoller.h"
#include "IMouseController.h"

#define DO_CASE(x) case x:{_SetProfile<x>();break;}

namespace Engine
{
  Layer_InputHandler::Layer_InputHandler(MessageBus * a_pMsgBus)
    : Layer(a_pMsgBus)
    , m_eventPoller(Framework::Instance()->GetEventPoller())
    , m_mouseController(Framework::Instance()->GetMouseController())
    , m_xMouseRotRate(1.0f)
    , m_yMouseRotRate(1.0f)
  {
    Bind(IC_MOUSE_MOTION, MT_Input_MouseMove, new MessageSub<MT_GUI_MouseMove>());
    Bind(IC_MOUSE_WHEEL_UP, MT_Input_MouseWheelUp, new MessageSub<MT_GUI_MouseWheelUp>());
    Bind(IC_MOUSE_WHEEL_DOWN, MT_Input_MouseWheelDown, new MessageSub<MT_GUI_MouseWheelDown>());
    Bind(IC_MOUSE_BUTTON_LEFT, MT_Input_MouseButtonDown, new MessageSub<MT_GUI_MouseButtonDown>());
    Bind(IC_MOUSE_BUTTON_LEFT, MT_Input_MouseButtonUp, new MessageSub<MT_GUI_MouseButtonUp>());
    Bind(IC_MOUSE_BUTTON_RIGHT, MT_Input_MouseButtonDown, new MessageSub<MT_GUI_MouseButtonDown>());
    Bind(IC_MOUSE_BUTTON_RIGHT, MT_Input_MouseButtonUp, new MessageSub<MT_GUI_MouseButtonUp>());

    Bind(IC_UNKNOWN, MT_Input_Text, new MessageSub<MT_GUI_Text>());
  }

  Layer_InputHandler::~Layer_InputHandler()
  {

  }

  void Layer_InputHandler::HandleMessage(MessageSub<MT_Input_Text> * a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_UNKNOWN, MT_Input_Text);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
  }

  void  Layer_InputHandler::HandleMessage(MessageSub<MT_Input_KeyUp> * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->keyCode, MT_Input_KeyUp);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
  }

  void  Layer_InputHandler::HandleMessage(MessageSub<MT_Input_KeyDown> * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->keyCode, MT_Input_KeyDown);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
  }

  void  Layer_InputHandler::HandleMessage(MessageSub<MT_Input_MouseButtonUp> * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->button, MT_Input_MouseButtonUp);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
  }

  void  Layer_InputHandler::HandleMessage(MessageSub<MT_Input_MouseButtonDown> * a_pMsg)
  {
    uint64_t mapKey = PackKey(a_pMsg->button, MT_Input_MouseButtonDown);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
  }

  void  Layer_InputHandler::HandleMessage(MessageSub<MT_Input_MouseWheelUp> * a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_MOUSE_WHEEL_UP, MT_Input_MouseWheelUp);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
  }

  void  Layer_InputHandler::HandleMessage(MessageSub<MT_Input_MouseWheelDown> * a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_MOUSE_WHEEL_DOWN, MT_Input_MouseWheelDown);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
  }

  void  Layer_InputHandler::HandleMessage(MessageSub<MT_Input_MouseMove> * a_pMsg)
  {
    uint64_t mapKey = PackKey(IC_MOUSE_MOTION, MT_Input_MouseMove);
    auto it = m_bindings.find(mapKey);
    if (it != m_bindings.end())
    {
      Message * pMsg = it->second->Clone();
      TranslateMessage(pMsg, a_pMsg);
      Post(pMsg);
    }
    a_pMsg->handled = true;
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

  uint64_t Layer_InputHandler::PackKey(uint32_t a_inputCode, MessageType a_messageType)
  {
    return (uint64_t(a_inputCode) << 32) | a_messageType;
  }

  void Layer_InputHandler::ClearBindings()
  {
    m_bindings.clear();
  }

  void Layer_InputHandler::Bind(InputCode a_inputCode, MessageType a_event, Message * a_binding)
  {
    m_bindings.insert(PackKey(a_inputCode, a_event), std::shared_ptr<Message>(a_binding));
  }

  /*template<>
  void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_Menu>()
  {
    m_bindings.clear();

    Bind(IC_MOUSE_MOTION, MT_Input_OtherMouseEvent, MT_GUI_MouseMove);
    Bind(IC_MOUSE_WHEEL_UP, MT_Input_OtherMouseEvent, MT_GUI_MouseWheelUp);
    Bind(IC_MOUSE_WHEEL_DOWN, MT_Input_OtherMouseEvent, MT_GUI_MouseWheelDown);
    Bind(IC_MOUSE_BUTTON_LEFT, MT_Input_ButtonDown, MT_GUI_MouseButtonDown);
    Bind(IC_MOUSE_BUTTON_LEFT, MT_Input_ButtonUp, MT_GUI_MouseButtonUp);

    Bind(IC_UNKNOWN, MT_Input_Text, MT_GUI_Text);

    BindKeyDown(IC_KEY_TAB, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_LEFT, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_RIGHT, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_UP, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_DOWN, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_PAGEUP, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_PAGEDOWN, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_HOME, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_END, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_INSERT, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_DELETE, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_BACKSPACE, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_SPACE, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_ENTER, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_KPENTER, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_A, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_C, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_V, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_X, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_Y, true, MT_GUI_KeyDown);
    BindKeyDown(IC_KEY_Z, true, MT_GUI_KeyDown);

    Bind(IC_KEY_TAB, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_LEFT, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_RIGHT, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_UP, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_DOWN, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_PAGEUP, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_PAGEDOWN, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_HOME, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_END, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_INSERT, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_DELETE, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_BACKSPACE, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_SPACE, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_ENTER, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_KPENTER, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_A, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_C, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_V, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_X, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_Y, MT_Input_KeyUp, MT_GUI_KeyUp);
    Bind(IC_KEY_Z, MT_Input_KeyUp, MT_GUI_KeyUp);

    Bind(IC_KEY_ESC, MT_Input_KeyDown, MT_GoBack);

    m_mouseController->Release();
  }*/

  void Layer_InputHandler::Update(float a_dt)
  {
    while (true)
    {
      Message * pMsg = m_eventPoller->NextEvent();
      if (pMsg == nullptr)
        break;

      pMsg->Submit(this);
      if (pMsg->handled)
        delete pMsg;
      else
        Post(pMsg);
    }
  }
}
