#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include "Utility.h"

#define TEXT_INPUT_TEXT_SIZE 32

#define START_BIT 13
#undef NEW_COUNTER
#define NEW_COUNTER MessageClassCounter
INIT_COUNTER(START_BIT)

namespace Engine
{
  enum MessageClass : uint32_t
  {
    MC_Window       = BIT(COUNTER),
    MC_Input        = BIT(COUNTER),
    MC_Text         = BIT(COUNTER),
    MC_Keyboard     = BIT(COUNTER),
    MC_Mouse        = BIT(COUNTER),
    MC_State        = BIT(COUNTER),
    MC_GUI          = BIT(COUNTER),
    MC_Game         = BIT(COUNTER)
  };

#undef NEW_COUNTER
#define NEW_COUNTER MessageTypeCounter
  INIT_COUNTER(0)

    enum MessageType : uint32_t
  {
    MT_None                   = (COUNTER),
    MT_GoBack                 = (COUNTER),       //typically at least bound to escape key

    MT_GUI_MouseMove          = (COUNTER | MC_GUI),
    MT_GUI_MouseButtonUp      = (COUNTER | MC_GUI),
    MT_GUI_MouseButtonDown    = (COUNTER | MC_GUI),
    MT_GUI_KeyUp              = (COUNTER | MC_GUI),
    MT_GUI_KeyDown            = (COUNTER | MC_GUI),
    MT_GUI_MouseWheelUp       = (COUNTER | MC_GUI),
    MT_GUI_MouseWheelDown     = (COUNTER | MC_GUI),

    MT_GUI_Text               = (COUNTER | MC_GUI | MC_Text),

    MT_Window_Shown           = (COUNTER | MC_Window), //< Window has been shown 
    MT_Window_Hidden          = (COUNTER | MC_Window), //< Window has been hidden 
    MT_Window_Exposed         = (COUNTER | MC_Window), //< Window has been exposed and should be redrawn 
    MT_Window_Moved           = (COUNTER | MC_Window), //< Window has been moved to data1: data2                                  
    MT_Window_Resized         = (COUNTER | MC_Window), //< Window has been resized to data1xdata2 
    MT_Window_Sized_Changed   = (COUNTER | MC_Window), //< The window size has changed: either as a result of an API call or through the system or user changing the window size. 
    MT_Window_Minimized       = (COUNTER | MC_Window), //< Window has been minimized 
    MT_Window_Maximized       = (COUNTER | MC_Window), //< Window has been maximized 
    MT_Window_Restored        = (COUNTER | MC_Window), //< Window has been restored to normal size and position 
    MT_Window_Enter           = (COUNTER | MC_Window), //< Window has gained mouse focus 
    MT_Window_Leave           = (COUNTER | MC_Window), //< Window has lost mouse focus 
    MT_Window_Focus_Gained    = (COUNTER | MC_Window), //< Window has gained keyboard focus 
    MT_Window_Focus_Lost      = (COUNTER | MC_Window), //< Window has lost keyboard focus 
    MT_Window_Close           = (COUNTER | MC_Window), //< The window manager requests that the window be closed 
    MT_Window_Take_Focus      = (COUNTER | MC_Window), //< Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow: or ignore) 
    MT_Window_Hit_Test        = (COUNTER | MC_Window), //< Window had a hit test that wasn't SDL_HITTEST_NORMAL. 

    MT_Input_Text             = (COUNTER | MC_Input | MC_Text),
    MT_Input_KeyUp            = (COUNTER | MC_Input | MC_Keyboard),
    MT_Input_KeyDown          = (COUNTER | MC_Input | MC_Keyboard),
    MT_Input_KeyDown_Repeat   = (COUNTER | MC_Input | MC_Keyboard),

    MT_Input_ButtonUp         = (COUNTER | MC_Input | MC_Mouse),
    MT_Input_ButtonDown       = (COUNTER | MC_Input | MC_Mouse),
    MT_Input_OtherMouseEvent  = (COUNTER | MC_Input | MC_Mouse),

    MT_State_None             = (COUNTER | MC_State),
    MT_State_Loading          = (COUNTER | MC_State),
    MT_State_Game             = (COUNTER | MC_State),
    MT_State_Menu             = (COUNTER | MC_State),
    MT_State_DebugOverlay     = (COUNTER | MC_State),
    MT_State_Elevator         = (COUNTER | MC_State),

    MT_Rotate                 = (COUNTER | MC_Game),
    MT_Activate               = (COUNTER | MC_Game),
    MT_Back                   = (COUNTER | MC_Game),
    MT_BeginFire              = (COUNTER | MC_Game),
    MT_EndFire                = (COUNTER | MC_Game),
    MT_BeginAltFire           = (COUNTER | MC_Game),
    MT_EndAltFire             = (COUNTER | MC_Game),
    MT_BeginForward           = (COUNTER | MC_Game),
    MT_EndForward             = (COUNTER | MC_Game),
    MT_BeginMoveBack          = (COUNTER | MC_Game),
    MT_EndMoveBack            = (COUNTER | MC_Game),
    MT_BeginTurnLeft          = (COUNTER | MC_Game),
    MT_EndTurnLeft            = (COUNTER | MC_Game),
    MT_BeginTurnRight         = (COUNTER | MC_Game),
    MT_EndTurnRight           = (COUNTER | MC_Game),
    MT_BeginMoveLeft          = (COUNTER | MC_Game),
    MT_EndMoveLeft            = (COUNTER | MC_Game),
    MT_BeginMoveRight         = (COUNTER | MC_Game),
    MT_EndMoveRight           = (COUNTER | MC_Game),
    MT_BeginActivate          = (COUNTER | MC_Game),
    MT_EndActivate            = (COUNTER | MC_Game),
    MT_RunOn                  = (COUNTER | MC_Game),
    MT_RunOff                 = (COUNTER | MC_Game),
    MT_ToggleRun              = (COUNTER | MC_Game),
    MT_ToggleMap              = (COUNTER | MC_Game),
    MT_GoToLevel              = (COUNTER | MC_Game),
  };
  static_assert(COUNTER < (1 << START_BIT), "Need more bits to store message enum values. Try incrementing START_BIT");
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
    uint32_t  type;
    uint16_t  code;
    uint16_t  modState;
  };

  struct TextInput
  {
    uint32_t  type;
    char      text[TEXT_INPUT_TEXT_SIZE];
  };

  struct MouseData
  {
    uint32_t type;
    uint16_t code;
    int32_t x;  //absolute/relative depending on if mouse grabbed
    int32_t y;
  };

  //-----------------------------------------------------------------------------------
  // Translated input data
  //-----------------------------------------------------------------------------------

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
      TextInput       text;
      WindowData      window;
      RotateData      rotate;
    };
  };
}
#endif