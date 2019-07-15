#ifndef SYSTEM_WINDOW_H
#define SYSTEM_WINDOW_H

#include "ErrorCodes.h"
#include "System.h"

class IWindow;

class System_Window : public System
{
public:

  System_Window(MessageBus * a_pMsgBus, IWindow *);
  ~System_Window();

  void OnAttach();
  void OnDetach();

  void Update();

  //Return bool: consumed
  bool HandleMessage(Message const &);

private:

   IWindow * m_pWindow;
};

#endif