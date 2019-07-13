#ifndef SYSTEM_WINDOW_H
#define SYSTEM_WINDOW_H

#include "System.h"

class System_Window : public System
{
public:

  System_Window(MessageBus * a_pMsgBus);
  ~System_Window();

  void Init();
  void OnAttach();
  void OnDetach();
  void ShutDown();

  void Update();

  //Return bool: consumed
  bool HandleMessage(Message const &) =0;

private:

};

#endif