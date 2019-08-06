#ifndef LAYER_INPUTHANDLER_H
#define LAYER_INPUTHANDLER_H

#include <stdint.h>

#include "DgMap_AVL.h"

#include "MessageBus.h"
#include "Message.h"
#include "Layer.h"
#include "InputCodes.h"

class IEventPoller;
class IMouseController;

#define ITEM
#define BINDING_PROFILES \
  ITEM(BP_None)\
  ITEM(BP_Loading)\
  ITEM(BP_Menu)\
  ITEM(BP_DebugOverlay)\
  ITEM(BP_Game)\
  ITEM(BP_Elevator)
#undef ITEM

class Layer_InputHandler : public Layer
{
  public:
#define ITEM(x) x,
  enum BindingProfile
  {
    BINDING_PROFILES
  };
#undef ITEM
public:

  ASSIGN_ID

  Layer_InputHandler(MessageBus *);
  ~Layer_InputHandler();

  void SetProfile(BindingProfile);
  void ClearBindings();

  bool HandleMessage(Message const &);
  void Update(float);

private:

  template<int BINDING>
  void _SetProfile(){}

#define ITEM(x) template<> void _SetProfile<x>();
  BINDING_PROFILES
#undef ITEM

  void GrabMouse();
  void ReleaseMouse();
  void SetMouseLookRate(float xRate, float yRate);

  uint64_t PackKey(uint32_t inputCode, uint32_t eventType);
  void UnpackKey(uint64_t mapKey, uint32_t & inputCode, uint32_t & eventType);
  void HandleTextEvent(Message const &);
  void HandleKeyEvent(Message const &);
  void HandleMouseEvent(Message const &);

  void Bind(InputCode inputCode, MessageType event, MessageType binding);
  void BindKeyDown(InputCode inputCode, bool repeat, MessageType binding);

private:

  IEventPoller *      m_eventPoller;
  IMouseController *  m_mouseController;

  float                               m_xMouseRotRate;
  float                               m_yMouseRotRate;
  Dg::Map_AVL<uint64_t, MessageType>  m_bindings;
  
};

#endif