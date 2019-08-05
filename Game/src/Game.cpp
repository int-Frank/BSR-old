#include <exception>

#include "Game.h"
#include "Options.h"
#include "Log.h"
#include "Framework/Framework.h"
#include "IWindow.h"

#include "Layer_Console.h"
#include "Layer_InputHandler.h"
#include "Layer_Window.h"
#include "Layer_ImGui.h"

Game * Game::s_instance = nullptr;

void Game::InitWindow()
{
  m_window = Framework::Instance()->GetWindow();
  if (m_window == nullptr)
    throw std::runtime_error("GetWindow() has returned a null pointer!");

  if (m_window->Init() != EC_None)
    throw std::runtime_error("Failed to initialise window!");
}

//Be sure to clean up if this fails.
void Game::Init()
{
  impl::Logger::Init("BSR");

  if (Framework::Init() != EC_None)
    throw std::runtime_error("Failed to initialise framework!");

  s_instance = new Game();
  s_instance->_Init();

  LOG_TRACE("Init complete!");
}

void Game::_Init()
{
  InitWindow();
  Framework::Instance()->InitImGui();

  auto pInputHandler = new Layer_InputHandler(&m_msgBus);
  pInputHandler->SetProfile(Layer_InputHandler::BP_TextInput);
  m_layerStack.PushLayer(pInputHandler, Layer_InputHandler::GetID());

  m_layerStack.PushLayer(new Layer_Window(&m_msgBus, m_window), Layer_Window::GetID());
  m_layerStack.PushLayer(new Layer_Console(&m_msgBus), Layer_Console::GetID());
  m_layerStack.PushLayer(new Layer_imgui(&m_msgBus), Layer_imgui::GetID());
}

void Game::ShutDown()
{
  if (Framework::ShutDown() != EC_None)
    LOG_ERROR("Failed to shut down framework!");

  delete s_instance;
  s_instance = nullptr;

  LOG_TRACE("Shutdown complete!");
}

Game * Game::Instance()
{
  BSR_ASSERT(s_instance != nullptr, "Game not initialised!");
  return s_instance;
}

bool Game::IsInitialised()
{
  return s_instance != nullptr;
}

Game::Game()
  : m_msgBus(m_layerStack)
  , m_window(nullptr)
  , m_shouldQuit(false)
{

}

Game::~Game()
{

}

void Game::Run()
{
  while (!m_shouldQuit)
  {
    m_msgBus.DispatchMessages();

    for (auto it = m_layerStack.begin(); it != m_layerStack.end(); it++)
    {
      it->second->Update();
    }

    auto it = m_layerStack.end();
    while (it != m_layerStack.begin())
    {
      it--;
      it->second->Render();
    }
  }
}

void Game::RequestQuit()
{
  m_shouldQuit = true;
}