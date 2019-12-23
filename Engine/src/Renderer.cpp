//@group Renderer

#include "Renderer.h"
#include "core_Log.h"
#include "core_Assert.h"
#include "Framework.h"
#include "core_Log.h"
#include "RendererAPI.h"
#include "RenderThread.h"

namespace Engine
{
  Renderer* Renderer::s_instance = nullptr;

  Renderer * Renderer::Instance()
  {
    BSR_ASSERT(s_instance != nullptr, "Renderer not initialised!");
    return s_instance;
  }

  bool Renderer::__Init()
  {
    return true;
  }

  bool Renderer::Init()
  {
    BSR_ASSERT(s_instance == nullptr, "Renderer instance already initialised!");
    s_instance = new Renderer();
    return s_instance->__Init();
  }

  void Renderer::ShutDown()
  {
    delete s_instance;
    s_instance = nullptr;
  }

  Renderer::Renderer()
  {
    
  }

  Renderer::~Renderer()
  {

  }

  void Renderer::Clear()
  {
    Engine::RenderState state = Engine::RenderState::Create();
    state.Set<Engine::RenderState::Attr::Type>(Engine::RenderState::Type::Command);
    state.Set<Engine::RenderState::Attr::Command>(Engine::RenderState::Command::Clear);

    RENDER_SUBMIT(state, []()
      {
        RendererAPI::Clear();
      });
  }

  void Renderer::Clear(float a_r, float a_g, float a_b, float a_a)
  {
    Engine::RenderState state = Engine::RenderState::Create();
    state.Set<Engine::RenderState::Attr::Type>(Engine::RenderState::Type::Command);
    state.Set<Engine::RenderState::Attr::Command>(Engine::RenderState::Command::Clear);

    RENDER_SUBMIT(state, [r = a_r, g = a_g, b = a_b, a = a_a]()
      {
        RendererAPI::Clear(r, g, b, a);
      });
  }

  void Renderer::SetClearColor(float a_r, float a_g, float a_b, float a_a)
  {
    Engine::RenderState state = Engine::RenderState::Create();
    state.Set<Engine::RenderState::Attr::Type>(Engine::RenderState::Type::Command);
    state.Set<Engine::RenderState::Attr::Command>(Engine::RenderState::Command::SetClearColor);

    RENDER_SUBMIT(state, [r = a_r, g = a_g, b = a_b, a = a_a]()
      {
        RendererAPI::SetClearColor(r, g, b, a);
      });
  }


  void Renderer::BeginScene()
  {
    m_group.Reset();
  }

  void Renderer::EndScene()
  {

  }

  void * Renderer::Allocate(uint32_t a_size)
  {
    return m_commandQueue.Allocate(a_size);
  }

  void Renderer::SwapBuffers()
  {
    m_commandQueue.Swap();
  }

  void Renderer::ExecuteRenderCommands()
  {
    m_commandQueue.Execute();
  }

  void Renderer::BeginNewGroup()
  {
    m_group.BeginNewGroup();
  }

  void Renderer::EndCurrentGroup()
  {
    m_group.EndCurrentGroup();
  }
}