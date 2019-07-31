
#include <cstring>

#include "Layer_InputHandler.h"
#include "Framework/Framework.h"
#include "Log.h"
#include "InputCodes.h"
#include "Message.h"

#include "IEventPoller.h"
#include "IMouseController.h"

#define DO_CASE(x) case x:{_SetProfile<x>();break;}

Layer_InputHandler::Layer_InputHandler(MessageBus * a_pMsgBus)
  : Layer(a_pMsgBus)
  , m_eventPoller(Framework::Instance()->GetEventPoller())
  , m_mouseController(Framework::Instance()->GetMouseController())
  , m_xMouseRotRate(1.0f)
  , m_yMouseRotRate(1.0f)
{

}

Layer_InputHandler::~Layer_InputHandler()
{

}

bool Layer_InputHandler::HandleMessage(Message const & a_msg)
{
  bool result = true;
  if ((a_msg.type & MC_Input))
  {
    if (a_msg.type & MC_Text)
      HandleTextEvent(a_msg);
    else if (a_msg.type & MC_Keyboard)
      HandleKeyEvent(a_msg);
    else if (a_msg.type & MC_Mouse)
      HandleMouseEvent(a_msg);
    else
      result = false;
  }
  else
    result = false;

  return result;
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

uint64_t Layer_InputHandler::PackKey(uint32_t a_inputCode, uint32_t a_eventType)
{
  return (uint64_t(a_inputCode) << 32) | a_eventType;
}

void Layer_InputHandler::UnpackKey(uint64_t a_mapKey, 
                                    uint32_t & a_inputCode, 
                                    uint32_t & a_eventType)
{
  a_inputCode = uint32_t(a_mapKey >> 32);
  a_eventType = uint32_t(a_mapKey);
}

void Layer_InputHandler::SetProfile(BindingProfile a_prof)
{
  switch (a_prof)
  {
#define ITEM(x) case x:{_SetProfile<x>();break;}
    BINDING_PROFILES
#undef ITEM
    default:
    {
      LOG_WARN("Unrecognised binding profile: {}", a_prof);
      break;
    }
  }
}

void Layer_InputHandler::ClearBindings()
{
  _SetProfile<BP_None>();
}

void Layer_InputHandler::Bind(InputCode a_inputCode, MessageType a_event, MessageType a_binding)
{
  m_bindings.insert(PackKey(a_inputCode, a_event), a_binding);
}

void Layer_InputHandler::BindKeyDown(InputCode a_inputCode, bool a_repeat, MessageType a_binding)
{
  m_bindings.insert(PackKey(a_inputCode, MT_KeyDown), a_binding);
  if (a_repeat)
    m_bindings.insert(PackKey(a_inputCode, MT_KeyDown_Repeat), a_binding);
}

template<>
void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_None>()
{
  m_bindings.clear();
}

template<>
void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_Loading>()
{
  m_bindings.clear();
  m_mouseController->Grab();
}

template<>
void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_Menu>()
{
  m_bindings.clear();

  Bind(IC_MOUSE_MOTION, MT_OtherMouseEvent, MT_MouseMove);
  Bind(IC_MOUSE_WHEEL_UP, MT_OtherMouseEvent, MT_ModifyUp);
  Bind(IC_MOUSE_WHEEL_DOWN, MT_OtherMouseEvent, MT_ModifyDown);
  Bind(IC_MOUSE_BUTTON_LEFT, MT_ButtonDown, MT_PointSelect);

  BindKeyDown(IC_KEY_UP, true, MT_PreviousItem);
  BindKeyDown(IC_KEY_DOWN, true, MT_NextItem);
  BindKeyDown(IC_KEY_LEFT, true, MT_ModifyDown);
  BindKeyDown(IC_KEY_RIGHT, true, MT_ModifyUp);

  Bind(IC_KEY_ENTER, MT_KeyDown, MT_Select);
  Bind(IC_KEY_ESC, MT_KeyDown, MT_GoBack);

  m_mouseController->Release();
}

template<>
void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_TextInput>()
{
  m_bindings.clear();

  Bind(IC_UNKNOWN, MT_TextEvent, MT_Text);

  BindKeyDown(IC_KEY_ENTER, false, MT_Select);
  BindKeyDown(IC_KEY_BACKSPACE, true, MT_Backspace);
  BindKeyDown(IC_KEY_ESC, false, MT_Window_Close);

  m_mouseController->Release();
}

template<>
void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_DebugOverlay>()
{
  m_bindings.clear();
  m_mouseController->Release();
}

template<>
void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_Game>()
{
  m_bindings.clear();
  m_mouseController->Grab();
}

template<>
void Layer_InputHandler::_SetProfile<Layer_InputHandler::BP_Elevator>()
{
  m_bindings.clear();
  m_mouseController->Release();
}

void Layer_InputHandler::Update()
{
  Message msg;
  while (m_eventPoller->NextEvent(msg))
  {
    if (!HandleMessage(msg))
      Post(msg);
  }
}

void Layer_InputHandler::HandleTextEvent(Message const & a_msg)
{
  uint64_t mapKey = PackKey(IC_UNKNOWN, MT_TextEvent);
  auto it = m_bindings.find(mapKey);
  if (it != m_bindings.end())
  {
    Message msg;
    msg.type = MT_Text;
    strncpy(msg.text.text, a_msg.text.text, TEXT_INPUT_TEXT_SIZE);

    Post(msg);
  }
}

void Layer_InputHandler::HandleKeyEvent(Message const & a_msg)
{
  uint64_t mapKey = PackKey(uint32_t(a_msg.key.code), uint32_t(a_msg.type));
  auto it = m_bindings.find(mapKey);
  if (it != m_bindings.end())
  {
    Message msg;
    msg.type = it->second;

    Post(msg);
  }
}

void Layer_InputHandler::HandleMouseEvent(Message const & a_msg)
{
  uint64_t mapKey = PackKey(uint32_t(a_msg.mouse.code), uint32_t(a_msg.type));
  auto it = m_bindings.find(mapKey);
  if (it != m_bindings.end())
  {
    Message msg;
    msg.type = it->second;
    uint32_t mask = IC_MOUSE_MOTION 
      | IC_MOUSE_BUTTON_LEFT
      | IC_MOUSE_BUTTON_MIDDLE
      | IC_MOUSE_BUTTON_RIGHT
      | IC_MOUSE_BUTTON_X1
      | IC_MOUSE_BUTTON_X2;

    if ((a_msg.mouse.code & mask) != 0)
    {
      msg.mouse.x = a_msg.mouse.x;
      msg.mouse.y = a_msg.mouse.y;
    }
    Post(msg);
  }
}