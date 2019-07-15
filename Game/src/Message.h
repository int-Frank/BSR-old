#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include "Utility.h"

enum MessageClass
{
  MC_None           = 0,
  MC_Game           = BIT(0),
  MC_Input          = BIT(1),
  MC_Keyboard       = BIT(2),
  MC_Mouse          = BIT(3),
  MC_Menu           = BIT(4),
  MC_Window         = BIT(5)
};

enum MessageType
{
  MT_None,

  //Window
  MT_Window_Shown,          //< Window has been shown 
  MT_Window_Hidden,         //< Window has been hidden 
  MT_Window_Exposed,        //< Window has been exposed and should be redrawn 
  MT_Window_Moved,          //< Window has been moved to data1: data2                                  
  MT_Window_Resized,        //< Window has been resized to data1xdata2 
  MT_Window_Sized_Changed,  //< The window size has changed: either as a result of an API call or through the system or user changing the window size. 
  MT_Window_Minimized,      //< Window has been minimized 
  MT_Window_Maximized,      //< Window has been maximized 
  MT_Window_Restored,       //< Window has been restored to normal size and position 
  MT_Window_Enter,          //< Window has gained mouse focus 
  MT_Window_Leave,          //< Window has lost mouse focus 
  MT_Window_Focus_Gained,   //< Window has gained keyboard focus 
  MT_Window_Focus_Lost,     //< Window has lost keyboard focus 
  MT_Window_Close,          //< The window manager requests that the window be closed 
  MT_Window_Take_Focus,     //< Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow: or ignore) 
  MT_Window_Hit_Test,       //< Window had a hit test that wasn't SDL_HITTEST_NORMAL. 

  //Input
  MT_ButtonUp,
  MT_ButtonDown,
  MT_MouseWheel,
  MT_MouseMotion,

  //Menu bindings
  MT_Select,       //Enter, mouse left button
  MT_MouseMove,
  MT_NextItem,     //down arrow
  MT_PreviousItem, //up arrow
  MT_ModifyUp,     //right arrow, wheel up
  MT_ModifyDown,   //left arrow, wheel down

  MT_GoBack,       //typically at least bound to escape key

  //Game specific
  MT_Rotate,
  MT_Activate,
  MT_Back,
  MT_BeginFire,
  MT_EndFire,
  MT_BeginAltFire,
  MT_EndAltFire,
  MT_BeginForward,
  MT_EndForward,
  MT_BeginMoveBack,
  MT_EndMoveBack,
  MT_BeginTurnLeft,
  MT_EndTurnLeft,
  MT_BeginTurnRight,
  MT_EndTurnRight,
  MT_BeginMoveLeft,
  MT_EndMoveLeft,
  MT_BeginMoveRight,
  MT_EndMoveRight,
  MT_BeginActivate,
  MT_EndActivate,
  MT_RunOn,
  MT_RunOff,
  MT_ToggleRun,
  MT_ToggleMap,
  MT_GoToLevel
};

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
struct MouseButtonData
{
  uint32_t type;
  uint32_t code;
  int x;
  int y;
};

struct KeyData
{
  uint32_t type;
  uint32_t code;
  bool     repeat;
};

struct MouseMoveData
{
  uint32_t type;
  int x;
  int y;
  int xRel;
  int yRel;
};

//-----------------------------------------------------------------------------------
// Translated input data
//-----------------------------------------------------------------------------------
struct TMouseMoveData
{
  uint32_t type;
  int x;
  int y;
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

struct MouseWheelData
{
  uint32_t type;
  int32_t  y;
};

class Message
{
public:

  Message();

  void SetType(uint32_t  a_class, uint32_t a_type);
  uint32_t GetClass() const;
  uint32_t GetType() const;
  bool IsOfClass(MessageClass) const;

  union
  {
    uint32_t type;

    //Message specific storage
    MouseButtonData mouseButton;
    MouseMoveData   mouseMove;
    MouseWheelData  mouseWheel;
    KeyData         key;
    WindowData      window;
    TMouseMoveData  tMouseMove;
    RotateData      rotate;
  };
};

#endif