
//#define MORE_MESSAGE_TYPES ITEM(TestMessage)

#include "Engine.h"
#include "Renderer.h"
#include "RendererAPI.h"

#include "imgui.h"

class GameLayer : public Engine::Layer
{

public:
  static uint32_t GetID()
  {
    return 6;
  }

  uint32_t GetThisID() override
  {
    return 6;
  }

  GameLayer()
  {
    
  }

  void OnAttach() override
  {
    Engine::Renderer::SetClearColor(1.0f, 0.0f, 1.0f);
  }

  void OnDetach() override
  {
  }

  void Update(float a_dt) override
  {

  }

  void Render() override
  {
    Engine::Renderer::Clear(1.0f, 0.0f, 1.0f);
  }

private:
};

class Game : public Engine::Application
{
public:
  Game(Opts const & a_opts)
    : Application(a_opts)
  {
    GameLayer * pgl = new GameLayer();
    PushLayer(pgl);
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
