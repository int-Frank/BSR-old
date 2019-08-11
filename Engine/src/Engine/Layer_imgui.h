#ifndef LAYER_IMGUI_H
#define LAYER_IMGUI_H

#include "Message.h"
#include "Layer.h"

namespace Engine
{
  //The imgui layer is not responsible for initialising imgui, this is done in the
  //Framework class. The imgui layer is responsible for handling input and rendering.
  class Layer_imgui : public Layer
  {
  public:

    ASSIGN_ID

      Layer_imgui(MessageBus *);
    ~Layer_imgui();

    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseMove> *);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseButtonDown> *);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseButtonUp> *);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_KeyDown> *);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_KeyUp> *);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseWheelDown> *);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseWheelUp> *);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_Text> *);

    void Update(float);
    void Render();
    void NewFrame();

    void DoImGui();

  private:

    void SetMouseButton(uint32_t button, bool down);

  private:

    float m_dt;
  };
}

#endif