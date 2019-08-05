#ifndef LAYER_IMGUI_H
#define LAYER_IMGUI_H

#include "Message.h"
#include "Layer.h"

class Layer_imgui : public Layer
{
public:

  ASSIGN_ID

  Layer_imgui(MessageBus *);
  ~Layer_imgui();

  bool HandleMessage(Message const &);
  void Update();
  void Render();

private:

  void HandleMouseButtonPressed(Message const &);
  void HandleMouseButtonReleased(Message const &);
  void HandleMouseScrollUp(Message const &);
  void HandleMouseScrollDown(Message const &);
  void HandleMouseMove(Message const &);
  void HandleKeyPressed(Message const &);
  void HandleKeyReleased(Message const &);
  void HandleTextInput(Message const &);
  void HandleWindowEvent(Message const &);

};

#endif