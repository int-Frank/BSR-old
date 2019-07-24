#ifndef SYSTEM_INPUTHANDLER_H
#define SYSTEM_INPUTHANDLER_H

#include <stdint.h>

#include "DgMap_AVL.h"

#include "MessageBus.h"
#include "Message.h"
#include "System.h"

class IEventPoller;
class IMouseController;

#define ITEM
#define BINDING_PROFILES \
  ITEM(BP_None)\
  ITEM(BP_Loading)\
  ITEM(BP_Menu)\
  ITEM(BP_TextInput)\
  ITEM(BP_DebugOverlay)\
  ITEM(BP_Game)\
  ITEM(BP_Elevator)
#undef ITEM

class System_InputHandler : public System
{
#define ITEM(x) x,
  enum BindingProfile
  {
    BINDING_PROFILES
  };
#undef ITEM
public:

  System_InputHandler(MessageBus *);
  ~System_InputHandler();

  void SetProfile(BindingProfile);
  void ClearBindings();

  bool HandleMessage(Message const &);
  void Update();

private:

  template<int BINDING>
  void _SetProfile(){}

#define ITEM(x) template<> void _SetProfile<x>();
  BINDING_PROFILES
#undef ITEM

  void GrabMouse();
  void ReleaseMouse();
  void SetMouseLookRate(float xRate, float yRate);

  void SetKeyRepeat(bool);

  uint64_t PackKey(uint32_t inputCode, uint32_t eventType);
  void UnpackKey(uint64_t mapKey, uint32_t & inputCode, uint32_t & eventType);
  void HandleKeyEvent(Message const &);
  void HandleMouseEvent(Message const &);

protected:

  IEventPoller *      m_eventPoller;
  IMouseController *  m_mouseController;

  bool                               m_useKeyRepeat;
  float                              m_xMouseRotRate;
  float                              m_yMouseRotRate;
  Dg::Map_AVL<uint64_t, MessageType> m_bindings;
};

#endif