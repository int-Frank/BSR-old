
#define MORE_MESSAGE_TYPES\
  ITEM(TestMessage)

#include "Engine.h"

#include "imgui.h"

class GameLayer : public Engine::Layer
{
public:

private:
};

class Game : public Engine::Application
{
public:
  Game(Opts const & a_opts)
    : Application(a_opts)
  {
    LOG_TRACE("Game initialised!");
  }

  ~Game()
  {

  }

};

Engine::Application* Engine::CreateApplication()
{
  Application::Opts opts;
  return new Game(opts);
}
