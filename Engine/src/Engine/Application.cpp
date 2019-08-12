#include <exception>

#include "Application.h"
#include "Options.h"
#include "Log.h"
#include "../Framework/Framework.h"
#include "IWindow.h"
#include "BSR_Assert.h"
#include "Message.h"

#include "Layer_Console.h"
#include "Layer_InputHandler.h"
#include "Layer_Window.h"
#include "Layer_ImGui.h"

namespace Engine
{
  //------------------------------------------------------------------------------------
  // Application...
  //------------------------------------------------------------------------------------
  Application * Application::s_instance = nullptr;

  void Application::InitWindow()
  {
    m_pWindow = Framework::Instance()->GetWindow();
    if (m_pWindow == nullptr)
      throw std::runtime_error("GetWindow() has returned a null pointer!");

    if (m_pWindow->Init() != EC_None)
      throw std::runtime_error("Failed to initialise window!");
  }

  Application * Application::Instance()
  {
    return s_instance;
  }

  Application::Application()
    : m_msgBus(m_layerStack)
    , m_pWindow(nullptr)
    , m_shouldQuit(false)
  {
    BSR_ASSERT(s_instance == nullptr, "Error, Application already created!");
    s_instance = this;

    impl::Logger::Init("BSR");

    MessageTranslator::AddDefaultTranslators();

    if (Framework::Init() != EC_None)
      throw std::runtime_error("Failed to initialise framework!");

    InitWindow();

    Framework::ImGui_InitData imguiData;
    m_pWindow->GetDimensions(imguiData.window_w, imguiData.window_h);
    Framework::Instance()->InitImGui(imguiData);

    m_layerStack.PushLayer(new Layer_InputHandler(&m_msgBus), Layer_InputHandler::GetID());
    m_layerStack.PushLayer(new Layer_Window(&m_msgBus, m_pWindow), Layer_Window::GetID());
    m_layerStack.PushLayer(new Layer_Console(&m_msgBus), Layer_Console::GetID());
    m_layerStack.PushLayer(new Layer_imgui(&m_msgBus), Layer_imgui::GetID());

    LOG_TRACE("Application initialised!");
  }

  Application::~Application()
  {
    if (Framework::ShutDown() != EC_None)
      LOG_ERROR("Failed to shut down framework!");

    s_instance = nullptr;

    LOG_TRACE("Shutdown complete!");
  }

  void Application::Run()
  {
    while (!m_shouldQuit)
    {
      float dt = 1.0f / 60.0f;

      m_msgBus.DispatchMessages();

      for (auto it = m_layerStack.begin(); it != m_layerStack.end(); it++)
        it->second->Update(dt);

      Layer_imgui * imguiLayer = static_cast<Layer_imgui*>(m_layerStack.GetLayer(Layer_imgui::GetID()));
      imguiLayer->NewFrame();
      for (auto it = m_layerStack.begin(); it != m_layerStack.end(); it++)
        it->second->DoImGui();

      auto it = m_layerStack.end();
      while (it != m_layerStack.begin())
      {
        it--;
        it->second->Render();
      }
    }
  }

  void Application::RequestQuit()
  {
    m_shouldQuit = true;
  }
}
