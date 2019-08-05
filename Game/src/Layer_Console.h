#ifndef LAYER_CONSOLE_H
#define LAYER_CONSOLE_H

#include "Message.h"
#include "Layer.h"

class Layer_Console : public Layer
{
public:

  ASSIGN_ID

  Layer_Console(MessageBus *);
  ~Layer_Console();

  bool HandleMessage(Message const &);
  void Update();

};

#endif