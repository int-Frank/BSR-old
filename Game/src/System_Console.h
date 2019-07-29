#ifndef SYSTEM_CONSOLE_H
#define SYSTEM_CONSOLE_H

#include "Message.h"
#include "System.h"

class System_Console : public System
{
public:

  System_Console(MessageBus *);
  ~System_Console();

  bool HandleMessage(Message const &);
  void Update();

};

#endif