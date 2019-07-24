#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include "Utility.h"

#define START_BIT 13
enum MessageClass
{
  MC_Unspecified  = BIT((START_BIT)),
  MC_Window       = BIT((START_BIT + 1)),
  MC_Input        = BIT((START_BIT + 2)),
  MC_Keyboard     = BIT((START_BIT + 3)),
  MC_Mouse        = BIT((START_BIT + 4)),
  MC_State        = BIT((START_BIT + 5)),
  MC_Menu         = BIT((START_BIT + 6)),
  MC_Game         = BIT((START_BIT + 7)),
};

namespace impl
{
  int const MESSAGE_COUNTER_BASE = __COUNTER__;
}

#define COUNTER (__COUNTER__ - impl::MESSAGE_COUNTER_BASE - 1)
enum MessageType
{
  MT_None                 = (COUNTER | MC_Unspecified),
  MT_GoBack               = (COUNTER | MC_Unspecified),       //typically at least bound to escape key
  MT_RawTextKey           = (COUNTER | MC_Unspecified),

  MT_Window_Shown         = (COUNTER | MC_Window), //< Window has been shown 
  MT_Window_Hidden        = (COUNTER | MC_Window), //< Window has been hidden 
  MT_Window_Exposed       = (COUNTER | MC_Window), //< Window has been exposed and should be redrawn 
  MT_Window_Moved         = (COUNTER | MC_Window), //< Window has been moved to data1: data2                                  
  MT_Window_Resized       = (COUNTER | MC_Window), //< Window has been resized to data1xdata2 
  MT_Window_Sized_Changed = (COUNTER | MC_Window), //< The window size has changed: either as a result of an API call or through the system or user changing the window size. 
  MT_Window_Minimized     = (COUNTER | MC_Window), //< Window has been minimized 
  MT_Window_Maximized     = (COUNTER | MC_Window), //< Window has been maximized 
  MT_Window_Restored      = (COUNTER | MC_Window), //< Window has been restored to normal size and position 
  MT_Window_Enter         = (COUNTER | MC_Window), //< Window has gained mouse focus 
  MT_Window_Leave         = (COUNTER | MC_Window), //< Window has lost mouse focus 
  MT_Window_Focus_Gained  = (COUNTER | MC_Window), //< Window has gained keyboard focus 
  MT_Window_Focus_Lost    = (COUNTER | MC_Window), //< Window has lost keyboard focus 
  MT_Window_Close         = (COUNTER | MC_Window), //< The window manager requests that the window be closed 
  MT_Window_Take_Focus    = (COUNTER | MC_Window), //< Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow: or ignore) 
  MT_Window_Hit_Test      = (COUNTER | MC_Window), //< Window had a hit test that wasn't SDL_HITTEST_NORMAL. 

  MT_KeyUp                = (COUNTER | MC_Input | MC_Keyboard),
  MT_KeyDown              = (COUNTER | MC_Input | MC_Keyboard),

  MT_ButtonUp             = (COUNTER | MC_Input | MC_Mouse),
  MT_ButtonDown           = (COUNTER | MC_Input | MC_Mouse),
  MT_OtherMouseEvent      = (COUNTER | MC_Input | MC_Mouse),

  MT_State_None           = (COUNTER | MC_State),
  MT_State_Loading        = (COUNTER | MC_State),
  MT_State_Game           = (COUNTER | MC_State),
  MT_State_Menu           = (COUNTER | MC_State),
  MT_State_TextInput      = (COUNTER | MC_State),
  MT_State_DebugOverlay   = (COUNTER | MC_State),
  MT_State_Elevator       = (COUNTER | MC_State),

  MT_Select               = (COUNTER | MC_Menu),
  MT_MouseMove            = (COUNTER | MC_Menu),
  MT_NextItem             = (COUNTER | MC_Menu),
  MT_PreviousItem         = (COUNTER | MC_Menu),
  MT_ModifyUp             = (COUNTER | MC_Menu),
  MT_ModifyDown           = (COUNTER | MC_Menu),

  MT_Rotate               = (COUNTER | MC_Game),
  MT_Activate             = (COUNTER | MC_Game),
  MT_Back                 = (COUNTER | MC_Game),
  MT_BeginFire            = (COUNTER | MC_Game),
  MT_EndFire              = (COUNTER | MC_Game),
  MT_BeginAltFire         = (COUNTER | MC_Game),
  MT_EndAltFire           = (COUNTER | MC_Game),
  MT_BeginForward         = (COUNTER | MC_Game),
  MT_EndForward           = (COUNTER | MC_Game),
  MT_BeginMoveBack        = (COUNTER | MC_Game),
  MT_EndMoveBack          = (COUNTER | MC_Game),
  MT_BeginTurnLeft        = (COUNTER | MC_Game),
  MT_EndTurnLeft          = (COUNTER | MC_Game),
  MT_BeginTurnRight       = (COUNTER | MC_Game),
  MT_EndTurnRight         = (COUNTER | MC_Game),
  MT_BeginMoveLeft        = (COUNTER | MC_Game),
  MT_EndMoveLeft          = (COUNTER | MC_Game),
  MT_BeginMoveRight       = (COUNTER | MC_Game),
  MT_EndMoveRight         = (COUNTER | MC_Game),
  MT_BeginActivate        = (COUNTER | MC_Game),
  MT_EndActivate          = (COUNTER | MC_Game),
  MT_RunOn                = (COUNTER | MC_Game),
  MT_RunOff               = (COUNTER | MC_Game),
  MT_ToggleRun            = (COUNTER | MC_Game),
  MT_ToggleMap            = (COUNTER | MC_Game),
  MT_GoToLevel            = (COUNTER | MC_Game),
};
static_assert(COUNTER < (1 << START_BIT), "Need more bits to store message enum values. Try to increment START_BIT");
#undef COUNTER
#undef START_BIT

inline bool IsOfClass(MessageType a_type, MessageClass a_class)
{
  return (a_type & a_class) != 0;
}

/*
  -door opens
  -bullet hits wall
  -critical hit
  -guard alerted
*/
struct LocationalEvent
{
  uint32_t  type;
  float     vector[3]; //offset relative to the player
};

//-----------------------------------------------------------------------------------
// Input data
//-----------------------------------------------------------------------------------
struct KeyData
{
  uint32_t type;
  uint16_t code;
  bool     repeat;
};

struct MouseData
{
  uint32_t type;
  uint16_t code;
  int16_t x;  //absolute/relative depending on if mouse grabbed
  int16_t y;
};

//-----------------------------------------------------------------------------------
// Translated input data
//-----------------------------------------------------------------------------------
struct TMouseMoveData
{
  uint32_t type;
  int16_t x;
  int16_t y;
};

struct RotateData
{
  uint32_t type;
  float dPitch;
  float dYaw;
};

//-----------------------------------------------------------------------------------
// Window data
//-----------------------------------------------------------------------------------
struct WindowData
{
  uint32_t  type;
  int32_t   data1;
  int32_t   data2;
};

struct Message
{
  //Debug
  Message() : type(MT_None){}

  union
  {
    uint32_t type;

    //Message specific storage
    MouseData       mouse;
    KeyData         key;
    WindowData      window;
    TMouseMoveData  tMouseMove;
    RotateData      rotate;
  };
};

#endif