//@group Renderer


#include "Renderer.h"
#include "core_Log.h"
#include "core_Assert.h"
#include "Framework.h"
#include "core_Log.h"
#include "RendererAPI.h"



namespace Engine
{
  static void RenderThread(Ref<IWindow> a_window)
  {
    if (Framework::Instance()->GetGraphicsContext()->Init() != Core::ErrorCode::EC_None)
    {
      LOG_ERROR("Unable to set rendering context!");
      Engine::Renderer::Instance()->RenderThreadInitFailed();
      return;
    }

    RendererAPI::Init();
    Renderer::Instance()->RenderThreadFinishInit();

    while (!Renderer::Instance()->ShouldExit())
    {
      a_window->SwapBuffers();
      Renderer::Instance()->FinishRender();
      Renderer::Instance()->ExecuteRenderCommands();
    }
    if (Framework::Instance()->GetGraphicsContext()->ShutDown() != Core::ErrorCode::EC_None)
      LOG_ERROR("Trouble shutting down the rendering context!!");
    Renderer::Instance()->RenderThreadShutDown();
  }

  Renderer* Renderer::s_instance = nullptr;

  Renderer * Renderer::Instance()
  {
    BSR_ASSERT(s_instance != nullptr, "Renderer not initialised!");
    return s_instance;
  }

  bool Renderer::__Init(Ref<IWindow> a_window)
  {
    m_mutex[m_tmain_ind].lock();
    m_renderThread = std::thread(RenderThread, a_window);
    {
      std::mutex mutex_start;
      std::unique_lock<std::mutex> lck(mutex_start);
      m_cv.wait(lck, [this]
        {
          return m_treturnCode != ReturnCode::None;
        });
    }
    return m_treturnCode == ReturnCode::Ready;
  }

  bool Renderer::Init(Ref<IWindow> a_window)
  {
    BSR_ASSERT(s_instance == nullptr, "Renderer instance already initialised!");
    s_instance = new Renderer();
    return s_instance->__Init(a_window);
  }

  void Renderer::ShutDown()
  {
    delete s_instance;
    s_instance = nullptr;
  }

  Renderer::Renderer()
    : m_treturnCode(ReturnCode::None)
    , m_tmain_ind(1)
    , m_trender_ind(0)
    , m_shouldExit(false)
  {
    
  }

  Renderer::~Renderer()
  {
    m_shouldExit = true;
    m_mutex[m_tmain_ind].unlock();
    m_renderThread.join();
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

  void Renderer::RenderThreadFinishInit()
  {
    m_mutex[m_trender_ind].lock();
    m_treturnCode = ReturnCode::Ready;
    m_cv.notify_all();
  }

  void Renderer::RenderThreadInitFailed()
  {
    m_treturnCode = ReturnCode::Fail;
    m_cv.notify_all();
  }

  void Renderer::RenderThreadShutDown()
  {
    m_mutex[m_trender_ind].unlock();
  }

  void Renderer::SyncAndHoldRenderThread()
  {
    int other = (m_tmain_ind + 1) % 2;
    m_mutex[other].lock();
  }

  void Renderer::SwapBuffers()
  {
    m_commandQueue.Swap();
  }

  void Renderer::ReleaseRenderThread()
  {
    int other = (m_tmain_ind + 1) % 2;
    m_mutex[m_tmain_ind].unlock();
    m_tmain_ind = other;
  }

  //m_mutex2 locked;
  void Renderer::FinishRender()
  {
    int other = (m_trender_ind + 1) % 2;
    m_mutex[m_trender_ind].unlock();
    m_mutex[other].lock(); //Main is doing work while we try to lock
    m_trender_ind = other;
  }

  void Renderer::ExecuteRenderCommands()
  {
    m_commandQueue.Execute();
  }

  bool Renderer::ShouldExit() const
  {
    return m_shouldExit;
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