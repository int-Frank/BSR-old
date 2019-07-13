#include <exception>

#include "Game.h"
#include "Options.h"
#include "Log.h"
#include "Framework/Framework.h"
#include "IWindow.h"

Game * Game::s_instance = nullptr;

//Be sure to clean up if this fails.
void Game::Init()
{
  impl::Logger::Init("BSR");
  s_instance = new Game();

  if (Framework::Instance()->Init() != EC_None)
    throw std::runtime_error("Failed to initialise framework!");

  s_instance->m_window = Framework::Instance()->GetWindow();
  if (s_instance->m_window->Init() != EC_None)
    throw std::runtime_error("Failed to initialise window!");

  LOG_DEBUG("Init complete!");
}

void Game::ShutDown()
{
  if (Framework::Instance()->ShutDown() != EC_None)
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
{

}

Game::~Game()
{

}

void Game::Run()
{

}