#include "Layer_imgui.h"
#include "imgui.h"
#include "Framework/imgui_impl_opengl3.h"
#include "InputCodes.h"

Layer_imgui::Layer_imgui(MessageBus * a_msgBus)
  : Layer(a_msgBus)
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
    else if (a_msg.type == MT_GUI_MouseWheel_Up)
    {
      ImGuiIO& io = ImGui::GetIO();
      io.MouseWheel += 1;
    }
    else if (a_msg.type == MT_GUI_MouseWheel_Down)
    {
      ImGuiIO& io = ImGui::GetIO();
      io.MouseWheel -= 1;
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
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  io.DeltaTime = a_dt;

  static bool show = true;
  ImGui::ShowDemoWindow(&show);

}

void Layer_imgui::Render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
