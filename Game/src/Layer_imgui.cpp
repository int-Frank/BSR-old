#include "Layer_imgui.h"
#include "imgui.h"
#include "Framework/imgui_impl_opengl3.h"

Layer_imgui::Layer_imgui(MessageBus * a_msgBus)
  : Layer(a_msgBus)
{
}

Layer_imgui::~Layer_imgui()
{
}

bool Layer_imgui::HandleMessage(Message const & a_msg)
{
  return false;
}

void Layer_imgui::Update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

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
