
#define MORE_MESSAGE_TYPES\
  ITEM(TestMessage)

#include "Engine.h"

#include "imgui.h"

class Game : public Engine::Application
{
public:
  Game()
  {
    LOG_TRACE("Game initialised!");
  }

  ~Game()
  {

  }

};

Engine::Application* Engine::CreateApplication()
{
  return new Game();
}