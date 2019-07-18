#ifndef SYSTEM_INPUTHANDLER_H
#define SYSTEM_INPUTHANDLER_H

#include <stdint.h>

#include "DgMap_AVL.h"

#include "MessageBus.h"
#include "Message.h"
#include "System.h"

class IEventPoller;
class IMouseController;

class System_InputHandler : public System
{
  enum BindingProfile
  {
    BP_None,
    BP_Loading,
    BP_Menu,
    BP_DebugOverlay,
    BP_Game,
    BP_Elevator
  };
public:

  System_InputHandler(MessageBus *);
  ~System_InputHandler();

  void SetProfile(BindingProfile);

private:

  void ClearBindings();

  template<int BINDING>
  void SetProfile(){}

  //For mouse motion just set the keyState to KS_NONE
  //inputCode: Class|Type eg KeyCode|MT_ButtonUp
  void SetBinding(MessageType messageType, uint32_t inputCode, uint32_t keyState);

  bool HandleMessage(Message const &);

  void GrabMouse();
  void ReleaseMouse();
  void SetMouseLook(float xSpeed, float ySpeed);

  uint64_t PackKey(uint32_t inputCode, uint32_t type);
  void UnpackKey(uint64_t mapKey, uint32_t & inputCode, uint32_t & type);
  void HandleInputEvent(Message const &);
  void RetrieveAndSend(uint64_t mapKey);

  virtual void HandleMouseMove(Message const &);

protected:

  IEventPoller * m_eventPoller;
  IMouseController * m_mouseController;

  float                              m_xMouseSpeed;
  float                              m_yMouseSpeed;
  Dg::Map_AVL<uint64_t, MessageType> m_messageMap;
};

#endif