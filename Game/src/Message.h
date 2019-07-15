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

  //Application
  MT_QuitRequest,

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

uint32_t CombineMessageParts(uint32_t, uint32_t);
uint32_t GetMessageClass(uint32_t);
uint32_t GetMessageType(uint32_t);

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

struct Message
{
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