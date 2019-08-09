#include "Layer_imgui.h"
#include "imgui.h"
#include "../Framework/imgui_impl_opengl3.h"
#include "InputCodes.h"
#include "Options.h"
#include "BSR_Assert.h"

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

  bool Layer_imgui::HandleMessage(Message const & a_msg)
  {
    if (a_msg.type & MC_GUI)
    {
      if (a_msg.type == MT_GUI_MouseMove)
      {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2((float)a_msg.mouse.x, (float)a_msg.mouse.y);
      }
      else if (a_msg.type == MT_GUI_MouseButtonDown)
      {
        SetMouseButton(a_msg.mouse.code, true);
      }
      else if (a_msg.type == MT_GUI_MouseButtonUp)
      {
        SetMouseButton(a_msg.mouse.code, false);
      }
      else if (a_msg.type == MT_GUI_MouseWheelUp)
      {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += 1;
      }
      else if (a_msg.type == MT_GUI_MouseWheelDown)
      {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel -= 1;
      }
      else if (a_msg.type == MT_GUI_KeyDown 
        || a_msg.type == MT_GUI_KeyUp)
      {
        ImGuiIO& io = ImGui::GetIO();
        BSR_ASSERT(a_msg.key.code >= 0 && a_msg.key.code < IM_ARRAYSIZE(io.KeysDown));
        io.KeysDown[a_msg.key.code] = (a_msg.type == MT_GUI_KeyDown);
        io.KeyShift = ((a_msg.key.modState & KM_SHIFT) != 0);
        io.KeyCtrl = ((a_msg.key.modState & KM_CTRL) != 0);
        io.KeyAlt = ((a_msg.key.modState & KM_ALT) != 0);
        io.KeySuper = ((a_msg.key.modState & KM_GUI) != 0);
      }
      else if (a_msg.type == MT_GUI_Text)
      {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharactersUTF8(a_msg.text.text);
      }
    }

    return false;
  }

  void Layer_imgui::SetMouseButton(uint32_t a_button, bool a_down)
  {
    int index = 0;
    switch (a_button)
    {
      case IC_MOUSE_BUTTON_LEFT:    index = 0; break;
      case IC_MOUSE_BUTTON_RIGHT:   index = 1; break;
      case IC_MOUSE_BUTTON_MIDDLE:  index = 2; break;
      case IC_MOUSE_BUTTON_X1:      index = 3; break;
      case IC_MOUSE_BUTTON_X2:      index = 4; break;
      default: break;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[index] = a_down;
  }

  void Layer_imgui::Update(float a_dt)
  {
    m_dt = a_dt;
  }

  void Layer_imgui::Render()
  {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

  void Layer_imgui::HandleMouseButtonPressed(Message const&)
  {
  }

  void Layer_imgui::HandleMouseButtonReleased(Message const&)
  {
  }

  void Layer_imgui::HandleMouseScrollUp(Message const&)
  {
  }

  void Layer_imgui::HandleMouseScrollDown(Message const&)
  {
  }

  void Layer_imgui::HandleMouseMove(Message const&)
  {
  }

  void Layer_imgui::HandleKeyPressed(Message const&)
  {
  }

  void Layer_imgui::HandleKeyReleased(Message const&)
  {
  }

  void Layer_imgui::HandleTextInput(Message const&)
  {
  }

  void Layer_imgui::HandleWindowEvent(Message const&)
  {
  }
}