
#include <cstring>

#include "System_InputHandler.h"
#include "Framework/Framework.h"
#include "Log.h"
#include "InputCodes.h"
#include "Message.h"

#include "IEventPoller.h"
#include "IMouseController.h"

#define DO_CASE(x) case x:{_SetProfile<x>();break;}

System_InputHandler::System_InputHandler(MessageBus * a_pMsgBus)
  : System(a_pMsgBus)
  , m_eventPoller(Framework::Instance()->GetEventPoller())
  , m_mouseController(Framework::Instance()->GetMouseController())
  , m_xMouseRotRate(1.0f)
  , m_yMouseRotRate(1.0f)
{

}

System_InputHandler::~System_InputHandler()
{

}

bool System_InputHandler::HandleMessage(Message const & a_msg)
{
  bool result = true;
  if (a_msg.type & MC_Text)
    HandleTextEvent(a_msg);
  if (a_msg.type & MC_Keyboard)
    HandleKeyEvent(a_msg);
  else if (a_msg.type & MC_Mouse)
    HandleMouseEvent(a_msg);
  else
    result = false;
  return result;
}

void System_InputHandler::GrabMouse()
{
  m_mouseController->Grab();
}

void System_InputHandler::ReleaseMouse()
{
  m_mouseController->Release();
}

void System_InputHandler::SetMouseLookRate(float a_xRate, float a_yRate)
{
  m_xMouseRotRate = a_xRate;
  m_yMouseRotRate = a_yRate;
}

uint64_t System_InputHandler::PackKey(uint32_t a_inputCode, uint32_t a_eventType)
{
  return (uint64_t(a_inputCode) << 32) | a_eventType;
}

void System_InputHandler::UnpackKey(uint64_t a_mapKey, 
                                    uint32_t & a_inputCode, 
                                    uint32_t & a_eventType)
{
  a_inputCode = uint32_t(a_mapKey >> 32);
  a_eventType = uint32_t(a_mapKey);
}

void System_InputHandler::SetProfile(BindingProfile a_prof)
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

void System_InputHandler::ClearBindings()
{
  _SetProfile<BP_None>();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_None>()
{
  m_bindings.clear();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Loading>()
{
  m_bindings.clear();
  m_mouseController->Grab();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Menu>()
{
  m_bindings.clear();

  m_bindings.insert(PackKey(IC_MOUSE_MOTION, MT_OtherMouseEvent), MT_MouseMove);
  m_bindings.insert(PackKey(IC_MOUSE_WHEEL_UP, MT_OtherMouseEvent), MT_ModifyUp);
  m_bindings.insert(PackKey(IC_MOUSE_WHEEL_DOWN, MT_OtherMouseEvent), MT_ModifyDown);
  m_bindings.insert(PackKey(IC_MOUSE_BUTTON_LEFT, MT_ButtonDown), MT_PointSelect);

  m_bindings.insert(PackKey(IC_KEY_UP, MT_KeyDown), MT_PreviousItem);
  m_bindings.insert(PackKey(IC_KEY_DOWN, MT_KeyDown), MT_NextItem);
  m_bindings.insert(PackKey(IC_KEY_LEFT, MT_KeyDown), MT_ModifyDown);
  m_bindings.insert(PackKey(IC_KEY_RIGHT, MT_KeyDown), MT_ModifyUp);

  m_bindings.insert(PackKey(IC_KEY_UP, MT_KeyDown_Repeat), MT_PreviousItem);
  m_bindings.insert(PackKey(IC_KEY_DOWN, MT_KeyDown_Repeat), MT_NextItem);
  m_bindings.insert(PackKey(IC_KEY_LEFT, MT_KeyDown_Repeat), MT_ModifyDown);
  m_bindings.insert(PackKey(IC_KEY_RIGHT, MT_KeyDown_Repeat), MT_ModifyUp);

  m_bindings.insert(PackKey(IC_KEY_ENTER, MT_KeyDown), MT_Select);
  m_bindings.insert(PackKey(IC_KEY_ESC, MT_KeyDown), MT_GoBack);

  m_mouseController->Release();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_TextInput>()
{
  m_bindings.clear();

  m_bindings.insert(PackKey(0, MT_TextInput), MT_TextInput);

  m_mouseController->Release();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_DebugOverlay>()
{
  m_bindings.clear();
  m_mouseController->Release();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Game>()
{
  m_bindings.clear();
  m_mouseController->Grab();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Elevator>()
{
  m_bindings.clear();
  m_mouseController->Release();
}

void System_InputHandler::Update()
{
  Message msg;
  while (m_eventPoller->NextEvent(msg))
  {
    if (!HandleMessage(msg))
      Post(msg);
  }
}

void System_InputHandler::HandleTextEvent(Message const & a_msg)
{
  uint64_t mapKey = PackKey(0, MT_TextInput);
  auto it = m_bindings.find(mapKey);
  if (it != m_bindings.end())
  {
    Message msg;
    msg.type = MT_TextInput;
    strncpy(msg.text.text, a_msg.text.text, TEXT_INPUT_TEXT_SIZE);

    Post(msg);
  }
}

void System_InputHandler::HandleKeyEvent(Message const & a_msg)
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

void System_InputHandler::HandleMouseEvent(Message const & a_msg)
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