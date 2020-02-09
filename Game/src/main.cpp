
#include <chrono>

#include "Engine.h"
#include "Renderer.h"
#include "RT_RendererAPI.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "RendererProgram.h"

#include "imgui.h"

class GameLayer : public Engine::Layer
{
public:

  ASSIGN_ID(256);
  GameLayer()
  {

  }

  void OnAttach() override
  {
    Engine::Renderer::SetClearColor(1.0f, 0.0f, 1.0f);

    float verts[] = 
    {
      0.0f, 0.5f,
      0.5f, -0.5f,
      -0.5f, -0.5f  
    };

    int indices[] = {0, 1, 2};

    m_vb = Engine::VertexBuffer::Create(verts, SIZEOF32(verts));
    m_vb->SetLayout(
      {
        { Engine::ShaderDataType::VEC2, "a_Position" }
      });

    m_ib = Engine::IndexBuffer::Create(indices, SIZEOF32(indices));
    //LOG_DEBUG("VertexBuffer: Ref ID: {}", m_vb->GetRefID().GetID());
    m_va = Engine::VertexArray::Create();

    m_va->AddVertexBuffer(m_vb);
    m_va->SetIndexBuffer(m_ib);

    m_prog = Engine::RendererProgram::Create();
    m_prog->Init(
      {
        { Engine::ShaderDomain::Vertex, Engine::StrType::Path, "D:/dev/projects/BSR/Game/src/vs.glsl" },
        { Engine::ShaderDomain::Fragment, Engine::StrType::Path, "D:/dev/projects/BSR/Game/src/fs.glsl" }
      });
  }

  void OnDetach() override
  {

  }

  void Update(float a_dt) override
  {
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //LOG_DEBUG("ASDF");
  }

  void Render() override
  {
    Engine::Renderer::Clear(1.0f, 0.0f, 1.0f);

    m_prog->Bind();
    m_va->Bind();

    Engine::Renderer::DrawIndexed(3, false);

  }

private:

  Engine::Ref<Engine::VertexBuffer>     m_vb;
  Engine::Ref<Engine::IndexBuffer>      m_ib;
  Engine::Ref<Engine::VertexArray>      m_va;
  Engine::Ref<Engine::RendererProgram>  m_prog;
};

class Game : public Engine::Application
{
public:
  Game(Opts const & a_opts)
    : Application(a_opts)
  {
    PushLayer(new GameLayer());

    LOG_TRACE("Game initialised!");
    //RequestQuit();
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
