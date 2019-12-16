//@group Renderer


#include "Renderer.h"
#include "core_Log.h"
#include "core_Assert.h"

static void RenderThread()
{
  Engine::Renderer::Instance()->RenderThreadInit();

  while (!Engine::Renderer::Instance()->ShouldExit())
  {
    Engine::Renderer::Instance()->FinishRender();
    Engine::Renderer::Instance()->ExecuteRenderCommands();
  }
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

  Renderer::Renderer()
    : m_ready(false)
    , m_tmain_ind(1)
    , m_trender_ind(0)
    , m_tmain_hasMutex3(true)
    , m_trender_hasMutex3(false)
  {
    m_mutex[m_tmain_ind].lock();
    m_mutex3.lock();
    m_renderThread = std::thread(RenderThread);
    {
      std::mutex mutex_start;
      std::unique_lock<std::mutex> lck(mutex_start);
      m_cv.wait(lck, [this]
        {
          return m_ready == true;
        });
    }
  }

  Renderer::~Renderer()
  {
    m_shouldExit = true;

    m_mutex[m_tmain_ind].unlock();
    if (m_tmain_hasMutex3)
      m_mutex3.unlock();

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
    m_ready = true;
    m_cv.notify_all();
  }

  void Renderer::RenderThreadShutDown()
  {
    m_mutex[m_trender_ind].unlock();
    if (m_trender_hasMutex3)
      m_mutex3.unlock();
  }

  //m_mutex1 locked;
  void Renderer::WaitForRenderer()
  {
    m_mutex[m_tmain_ind].unlock(); //Allow render thread through
    m_mutex[(m_tmain_ind + 1) % 2].lock();   //Wait for render thread to finish work
    if (m_tmain_hasMutex3)
      m_mutex3.unlock();
    else
      m_mutex3.lock();
    m_tmain_hasMutex3 = !m_tmain_hasMutex3;
    m_tmain_ind = (m_tmain_ind + 1) % 2;
  }

  //m_mutex2 locked;
  void Renderer::FinishRender()
  {
    m_mutex[(m_trender_ind + 1) % 2].lock();
    m_mutex[m_trender_ind].unlock();
    if (m_trender_hasMutex3)
      m_mutex3.unlock();
    else
      m_mutex3.lock();
    m_trender_hasMutex3 = !m_trender_hasMutex3;
    m_trender_ind = (m_trender_ind + 1) % 2;
  }

  void Renderer::ExecuteRenderCommands()
  {

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