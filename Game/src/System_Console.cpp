#include "System_Console.h"
#include "Log.h"

#define LOG_MESSAGE(...) LOG_TRACE(__VA_ARGS__)

System_Console::System_Console(MessageBus * a_msgBus)
  : System(a_msgBus)
{

}

System_Console::~System_Console()
{

}

void System_Console::Update()
{

}

bool System_Console::HandleMessage(Message const & a_msg)
{
  switch (a_msg.type)
  {
    case MT_None:                 {LOG_MESSAGE("MESSAGE: MT_None"); break;}
    case MT_GoBack:               {LOG_MESSAGE("MESSAGE: MT_GoBack"); break;}
    case MT_TextInput:            {LOG_MESSAGE("MESSAGE: MT_TextInput, '{}'", a_msg.text.text); break;}
    
    case MT_Window_Shown:         {LOG_MESSAGE("MESSAGE: MT_Window_Shown"); break;}
    case MT_Window_Hidden:        {LOG_MESSAGE("MESSAGE: MT_Window_Hidden"); break;}
    case MT_Window_Exposed:       {LOG_MESSAGE("MESSAGE: MT_Window_Exposed"); break;}
    case MT_Window_Moved:         {LOG_MESSAGE("MESSAGE: MT_Window_Moved"); break;}
    case MT_Window_Resized:       {LOG_MESSAGE("MESSAGE: MT_Window_Resized"); break;}
    case MT_Window_Sized_Changed: {LOG_MESSAGE("MESSAGE: MT_Window_Sized_Changed"); break;}
    case MT_Window_Minimized:     {LOG_MESSAGE("MESSAGE: MT_Window_Minimized"); break;}
    case MT_Window_Maximized:     {LOG_MESSAGE("MESSAGE: MT_Window_Maximized"); break;}
    case MT_Window_Restored:      {LOG_MESSAGE("MESSAGE: MT_Window_Restored"); break;}
    case MT_Window_Enter:         {LOG_MESSAGE("MESSAGE: MT_Window_Enter"); break;}
    case MT_Window_Leave:         {LOG_MESSAGE("MESSAGE: MT_Window_Leave"); break;}
    case MT_Window_Focus_Gained:  {LOG_MESSAGE("MESSAGE: MT_Window_Focus_Gained"); break;}
    case MT_Window_Focus_Lost:    {LOG_MESSAGE("MESSAGE: MT_Window_Focus_Lost"); break;}
    case MT_Window_Close:         {LOG_MESSAGE("MESSAGE: MT_Window_Close"); break;}
    case MT_Window_Take_Focus:    {LOG_MESSAGE("MESSAGE: MT_Window_Take_Focus"); break;}
    case MT_Window_Hit_Test:      {LOG_MESSAGE("MESSAGE: MT_Window_Hit_Test"); break;}

    case MT_KeyUp:                {LOG_MESSAGE("MESSAGE: MT_KeyUp"); break;}
    case MT_KeyDown:              {LOG_MESSAGE("MESSAGE: MT_KeyDown"); break;}
    case MT_KeyDown_Repeat:       {LOG_MESSAGE("MESSAGE: MT_KeyDown_Repeat"); break;}

    case MT_ButtonUp:             {LOG_MESSAGE("MESSAGE: MT_ButtonUp"); break;}
    case MT_ButtonDown:           {LOG_MESSAGE("MESSAGE: MT_ButtonDown"); break;}
    case MT_OtherMouseEvent:      {LOG_MESSAGE("MESSAGE: MT_OtherMouseEvent"); break;}

    case MT_State_None:           {LOG_MESSAGE("MESSAGE: MT_State_None"); break;}
    case MT_State_Loading:        {LOG_MESSAGE("MESSAGE: MT_State_Loading"); break;}
    case MT_State_Game:           {LOG_MESSAGE("MESSAGE: MT_State_Game"); break;}
    case MT_State_Menu:           {LOG_MESSAGE("MESSAGE: MT_State_Menu"); break;}
    case MT_State_TextInput:      {LOG_MESSAGE("MESSAGE: MT_State_TextInput"); break;}
    case MT_State_DebugOverlay:   {LOG_MESSAGE("MESSAGE: MT_State_DebugOverlay"); break;}
    case MT_State_Elevator:       {LOG_MESSAGE("MESSAGE: MT_State_Elevator"); break;}

    case MT_PointSelect:          {LOG_MESSAGE("MESSAGE: MT_PointSelect, x: {}, y: {}", a_msg.mouse.x, a_msg.mouse.y); break;}
    case MT_Select:               {LOG_MESSAGE("MESSAGE: MT_Select"); break;}
    case MT_MouseMove:            {LOG_MESSAGE("MESSAGE: MT_MouseMove, x: {}, y: {}", a_msg.mouse.x, a_msg.mouse.y); break;}
    case MT_NextItem:             {LOG_MESSAGE("MESSAGE: MT_NextItem"); break;}
    case MT_PreviousItem:         {LOG_MESSAGE("MESSAGE: MT_PreviousItem"); break;}
    case MT_ModifyUp:             {LOG_MESSAGE("MESSAGE: MT_ModifyUp"); break;}
    case MT_ModifyDown:           {LOG_MESSAGE("MESSAGE: MT_ModifyDown"); break;}
    default:                      {LOG_MESSAGE("MESSAGE: UNRECOGNISED");}
  }

  return false;
}