#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include "Utility.h"

enum MessageClass
{
  MC_None = 0,
  MC_Application    = BIT(0),
  MC_Input          = BIT(1),
  MC_Keyboard       = BIT(2),
  MC_Mouse          = BIT(3),
  MC_Menu           = BIT(4),
  MC_Game           = BIT(5)
};

enum class MessageType
{
  None,

  //Events
  Event_Input,
  Event_Window,
  Event_QuitRequest,

  //Menu bindings
  Select,       //Enter, mouse left button
  MouseMove,
  NextItem,     //down arrow
  PreviousItem, //up arrow
  ModifyUp,     //right arrow, wheel up
  ModifyDown,   //left arrow, wheel down

  GoBack,       //typically at least bound to escape key

  //Game specific
  Rotate,
  Activate,
  Back,
  BeginFire,
  EndFire,
  BeginAltFire,
  EndAltFire,
  BeginForward,
  EndForward,
  BeginMoveBack,
  EndMoveBack,
  BeginTurnLeft,
  EndTurnLeft,
  BeginTurnRight,
  EndTurnRight,
  BeginMoveLeft,
  EndMoveLeft,
  BeginMoveRight,
  EndMoveRight,
  BeginActivate,
  EndActivate,
  RunOn,
  RunOff,
  ToggleRun,
  ToggleMap,
  GoToLevel
};


/*
  -door opens
  -bullet hits wall
  -critical hit
  -guard alerted
*/
struct LocationalEvent
{
  float vector[3]; //offset relative to the player
};

//-----------------------------------------------------------------------------------
// Input data
//-----------------------------------------------------------------------------------
struct MouseButtonData
{
  uint32_t type; //IT_MOUSEBUTTON
  uint32_t state;
  uint32_t code;
  int x;
  int y;
};

struct KeyData
{
  uint32_t type; //IT_KEY
  uint32_t state;
  uint32_t code;
  bool     repeat;
};

struct MouseWheelData
{
  uint32_t type; //IT_MOUSEWHEEL
  int y;
};

struct MouseMoveData
{
  uint32_t type; //IT_MOUSEMOVE
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
  int x;
  int y;
};

struct RotateData
{
  float dPitch;
  float dYaw;
};

//-----------------------------------------------------------------------------------
// Window data
//-----------------------------------------------------------------------------------
struct WindowData
{
  uint32_t  code;
  int32_t   data1;
  int32_t   data2;
};

struct Message
{
  MessageType type;
  union
  {
    //Generic storage
    float     f32;
    uint32_t  uint32;
    int32_t   int32;

    //Message specific storage
    MouseButtonData mouseButton;
    MouseWheelData  mouseWheel;
    MouseMoveData   mouseMove;
    KeyData         key;
    WindowData      window;
    TMouseMoveData  tMouseMove;
    RotateData      rotate;
  };
};

#endif