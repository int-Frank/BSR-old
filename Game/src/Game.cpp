#include <exception>

#include "Game.h"
#include "Options.h"
#include "Log.h"
#include "Framework/Framework.h"
#include "IWindow.h"

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
  s_instance = new Game();

  if (Framework::Init() != EC_None)
    throw std::runtime_error("Failed to initialise framework!");

  s_instance->InitWindow();

  //Set up Window system and add to stack

  //Set up Input system and add to stack

  LOG_DEBUG("Init complete!");
}

void Game::ShutDown()
{
  if (Framework::ShutDown() != EC_None)
    LOG_ERROR("Failed to shut down framework!");

  delete s_instance;
  s_instance = nullptr;

  LOG_DEBUG("Shutdown complete!");
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
  : m_msgBus(m_systemStack)
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
  
  }
}

void Game::RequestQuit()
{
  m_shouldQuit = true;
}