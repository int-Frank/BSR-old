//@group Layers

#include "Layer_imgui.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "InputCodes.h"
#include "Options.h"
#include "core_Assert.h"

namespace Engine
{
  Layer_imgui::Layer_imgui(MessageBus * a_msgBus)
    : Layer(a_msgBus)
    , m_dt(1.0f / 60.0f)
  {
  }

  Layer_imgui::~Layer_imgui()
  {
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_MouseMove> * a_pMsg)
  {
    //ImGuiIO& io = ImGui::GetIO();
    //io.MousePos = ImVec2((float)a_pMsg->x, (float)a_pMsg->y);
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_MouseButtonDown> * a_pMsg)
  {
    //SetMouseButton(a_pMsg->button, true);
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_MouseButtonUp> * a_pMsg)
  {
    //SetMouseButton(a_pMsg->button, false);
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_KeyDown> * a_pMsg)
  {
    //ImGuiIO& io = ImGui::GetIO();
    //BSR_ASSERT(a_pMsg->keyCode >= 0 && a_pMsg->keyCode < IM_ARRAYSIZE(io.KeysDown));
    //io.KeysDown[a_pMsg->keyCode] = true;
    //io.KeyShift = ((a_pMsg->modState & KM_SHIFT) != 0);
    //io.KeyCtrl = ((a_pMsg->modState & KM_CTRL) != 0);
    //io.KeyAlt = ((a_pMsg->modState & KM_ALT) != 0);
    //io.KeySuper = ((a_pMsg->modState & KM_GUI) != 0);
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_KeyUp> * a_pMsg)
  {
    //ImGuiIO& io = ImGui::GetIO();
    //BSR_ASSERT(a_pMsg->keyCode >= 0 && a_pMsg->keyCode < IM_ARRAYSIZE(io.KeysDown));
    //io.KeysDown[a_pMsg->keyCode] = false;
    //io.KeyShift = ((a_pMsg->modState & KM_SHIFT) != 0);
    //io.KeyCtrl = ((a_pMsg->modState & KM_CTRL) != 0);
    //io.KeyAlt = ((a_pMsg->modState & KM_ALT) != 0);
    //io.KeySuper = ((a_pMsg->modState & KM_GUI) != 0);
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_MouseWheelUp> * a_pMsg)
  {
    //ImGuiIO& io = ImGui::GetIO();
    //io.MouseWheel += 1;
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_MouseWheelDown> * a_pMsg)
  {
    //ImGuiIO& io = ImGui::GetIO();
    //io.MouseWheel -= 1;
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::HandleMessage(MessageSub<MT_GUI_Text> * a_pMsg)
  {
    //ImGuiIO& io = ImGui::GetIO();
    //io.AddInputCharactersUTF8(a_pMsg->text);
    //a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_imgui::SetMouseButton(uint32_t a_button, bool a_down)
  {
    //int index = 0;
    //switch (a_button)
    //{
    //  case IC_MOUSE_BUTTON_LEFT:    index = 0; break;
    //  case IC_MOUSE_BUTTON_RIGHT:   index = 1; break;
    //  case IC_MOUSE_BUTTON_MIDDLE:  index = 2; break;
    //  case IC_MOUSE_BUTTON_X1:      index = 3; break;
    //  case IC_MOUSE_BUTTON_X2:      index = 4; break;
    //  default: break;
    //}
    //
    //ImGuiIO& io = ImGui::GetIO();
    //io.MouseDown[index] = a_down;
  }

  void Layer_imgui::Update(float a_dt)
  {
    m_dt = a_dt;
  }

  void Layer_imgui::Render()
  {
    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void Layer_imgui::NewFrame()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = m_dt;
  }

  void Layer_imgui::DoImGui()
  {
    static bool show = true;
    ImGui::ShowDemoWindow(&show);
  }
}