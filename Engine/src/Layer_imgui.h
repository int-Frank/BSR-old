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

    void HandleMessage(MessageSub<MT_GUI_MouseMove> *) override;
    void HandleMessage(MessageSub<MT_GUI_MouseButtonDown> *) override;
    void HandleMessage(MessageSub<MT_GUI_MouseButtonUp> *) override;
    void HandleMessage(MessageSub<MT_GUI_KeyDown> *) override;
    void HandleMessage(MessageSub<MT_GUI_KeyUp> *) override;
    void HandleMessage(MessageSub<MT_GUI_MouseWheelDown> *) override;
    void HandleMessage(MessageSub<MT_GUI_MouseWheelUp> *) override;
    void HandleMessage(MessageSub<MT_GUI_Text> *) override;

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