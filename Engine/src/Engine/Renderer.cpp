#include "Renderer.h"
#include "core_Log.h"
#include "core_Assert.h"

namespace Engine
{
  Renderer* Renderer::s_instance = nullptr;

  Renderer * Renderer::Instance()
  {
    BSR_ASSERT(s_instance != nullptr, "Renderer not initialised!");
    return s_instance;
  }

  void Renderer::Init()
  {
    BSR_ASSERT(s_instance == nullptr, "Renderer instance already initialised!");
    s_instance = new Renderer();
  }

  void Renderer::ShutDown()
  {
    delete s_instance;
    s_instance = nullptr;
  }

  void Renderer::BeginScene()
  {
    m_group.Reset();
  }

  void Renderer::EndScene()
  {

  }

  void Renderer::RenderAndWait()
  {

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