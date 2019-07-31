#ifndef SYSTEM_CONSOLE_H
#define SYSTEM_CONSOLE_H

#include "Message.h"
#include "Layer.h"

class Layer_Console : public Layer
{
public:

  Layer_Console(MessageBus *);
  ~Layer_Console();

  bool HandleMessage(Message const &);
  void Update();

};

#endif