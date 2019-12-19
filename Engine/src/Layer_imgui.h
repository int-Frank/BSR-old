//@group Layers

#ifndef EN_LAYER_IMGUI_H
#define EN_LAYER_IMGUI_H

#include "Message.h"
#include "Layer.h"

namespace Engine
{
  //The imgui layer is not responsible for initialising imgui, this is done in the
  //Framework class. The imgui layer is responsible for handling input and rendering.
  class Layer_imgui : public Layer
  {
    ASSIGN_ID
  public:

    Layer_imgui();
    ~Layer_imgui();

    void HandleMessage(Message *) override;
    void HandleMessage(Message_GUI_MouseMove*);
    void HandleMessage(Message_GUI_MouseButtonDown *);
    void HandleMessage(Message_GUI_MouseButtonUp *);
    void HandleMessage(Message_GUI_KeyDown *);
    void HandleMessage(Message_GUI_KeyUp *);
    void HandleMessage(Message_GUI_MouseWheelDown *);
    void HandleMessage(Message_GUI_MouseWheelUp *);
    void HandleMessage(Message_GUI_Text *);

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