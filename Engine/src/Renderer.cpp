//@group Renderer


#include "Renderer.h"
#include "core_Log.h"
#include "core_Assert.h"
#include "Framework.h"
#include "core_Log.h"

static void RenderThread()
{
  if (Engine::Framework::Instance()->GetGraphicsContext()->Init() != Core::ErrorCode::EC_None)
  {
    LOG_ERROR("Unable to set rendering context!");
    Engine::Renderer::Instance()->RenderThreadInitFailed();
    return;
  }

  Engine::Renderer::Instance()->RenderThreadInit();

  while (!Engine::Renderer::Instance()->ShouldExit())
  {
    Engine::Renderer::Instance()->FinishRender();
    Engine::Renderer::Instance()->ExecuteRenderCommands();
  }
  if (Engine::Framework::Instance()->GetGraphicsContext()->ShutDown() != Core::ErrorCode::EC_None)
    LOG_ERROR("Trouble shutting down the rendering context!!");
  Engine::Renderer::Instance()->RenderThreadShutDown();
}

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
    m_mutex[m_tmain_ind].lock();
    m_renderThread = std::thread(RenderThread);
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

  void Renderer::RenderThreadInit()
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