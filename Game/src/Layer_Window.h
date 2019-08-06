#ifndef LAYER_WINDOW_H
#define LAYER_WINDOW_H

#include "ErrorCodes.h"
#include "Layer.h"

class IWindow;

class Layer_Window : public Layer
{
public:

  ASSIGN_ID

  Layer_Window(MessageBus * a_pMsgBus, IWindow *);
  ~Layer_Window();

  void OnAttach();
  void OnDetach();

  void Update(float);

  //Return bool: consumed
  bool HandleMessage(Message const &);

private:

   IWindow * m_pWindow;
};

#endif