
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
  , m_useKeyRepeat(false)
{

}

System_InputHandler::~System_InputHandler()
{

}

bool System_InputHandler::HandleMessage(Message const & a_msg)
{
  bool result = true;
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
  SetKeyRepeat(false);
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Loading>()
{
  m_bindings.clear();
  SetKeyRepeat(false);
  m_mouseController->Grab();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Menu>()
{
  m_bindings.clear();
  m_bindings.insert(PackKey(IC_UP, MT_KeyDown), MT_PreviousItem);
  m_bindings.insert(PackKey(IC_DOWN, MT_KeyDown), MT_NextItem);
  m_bindings.insert(PackKey(IC_LEFT, MT_KeyDown), MT_ModifyDown);
  m_bindings.insert(PackKey(IC_RIGHT, MT_KeyDown), MT_ModifyUp);
  m_bindings.insert(PackKey(IC_MOUSEWHEEL_UP, MT_OtherMouseEvent), MT_ModifyUp);
  m_bindings.insert(PackKey(IC_MOUSEWHEEL_DOWN, MT_OtherMouseEvent), MT_ModifyDown);
  m_bindings.insert(PackKey(IC_MOUSEBUTTON_LEFT, MT_KeyDown), MT_Select);
  m_bindings.insert(PackKey(IC_RETURN, MT_KeyDown), MT_Select);
  m_bindings.insert(PackKey(IC_ESCAPE, MT_KeyDown), MT_GoBack);
  SetKeyRepeat(false);
  m_mouseController->Release();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_TextInput>()
{
  m_bindings.clear();
  m_bindings.insert(PackKey(IC_ESCAPE, MT_KeyDown), MT_GoBack);
  for (int i = IC_BACKSPACE; i <= IC_z; i++)
    m_bindings.insert(PackKey(InputCode(i), MT_KeyDown), MT_RawTextKey);
  for (int i = IC_a; i <= IC_z; i++)
  {
    m_bindings.insert(PackKey(InputCode(i | KM_LSHIFT), MT_KeyDown), MT_RawTextKey);
    m_bindings.insert(PackKey(InputCode(i | KM_RSHIFT), MT_KeyDown), MT_RawTextKey);
  }

  SetKeyRepeat(true);
  m_mouseController->Release();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_DebugOverlay>()
{
  m_bindings.clear();
  SetKeyRepeat(false);
  m_mouseController->Release();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Game>()
{
  m_bindings.clear();
  SetKeyRepeat(false);
  m_mouseController->Grab();
}

template<>
void System_InputHandler::_SetProfile<System_InputHandler::BP_Elevator>()
{
  m_bindings.clear();
  SetKeyRepeat(false);
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

void System_InputHandler::SetKeyRepeat(bool a_val)
{
  m_useKeyRepeat = a_val;
}

void System_InputHandler::HandleKeyEvent(Message const & a_msg)
{
  if (a_msg.key.repeat && ! m_useKeyRepeat)
    return;

  uint64_t mapKey = PackKey(uint32_t(a_msg.key.code), uint32_t(a_msg.type));
  auto it = m_bindings.find(mapKey);
  if (it != m_bindings.end())
  {
    Message msg;
    msg.type = it->second;

    if (it->second == MT_RawTextKey)
      msg.key.code = a_msg.key.code;

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
    if (a_msg.mouse.code == IC_MOUSE_MOTION)
    {
      msg.mouse.x = a_msg.mouse.x;
      msg.mouse.y = a_msg.mouse.y;
    }
    Post(msg);
  }
}