#include "Game.h"
#include "Options.h"
#include "Log.h"

Game * Game::s_instance = nullptr;

//Be sure to clean up if this fails.
void Game::Init()
{
  impl::Logger::Init("BSR");
  s_instance = new Game();

  LOG_DEBUG("Init complete!");
}

void Game::ShutDown()
{
  delete s_instance;
  s_instance = nullptr;

  LOG_DEBUG("Shutdown complete!");
}

bool Game::IsInitialised()
{
  return s_instance != nullptr;
}

Game * Game::Instance()
{
  return s_instance;
}

Game::Game()
{

}

Game::~Game()
{

}

void Game::Run()
{

}