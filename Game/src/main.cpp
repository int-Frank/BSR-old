
#include "Engine.h"
#include "Renderer.h"
#include "RendererAPI.h"

#include "imgui.h"

class GameLayer : public Engine::Layer
{
  ASSIGN_ID
public:

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
    PushLayer(new GameLayer());
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
