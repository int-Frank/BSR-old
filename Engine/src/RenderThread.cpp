//@group Renderer/RenderThread

#include "core_Assert.h"
#include "RenderThread.h"
#include "Framework.h"
#include "RT_RendererAPI.h"
#include "RenderThreadData.h"
#include "RT_BindingPoint.h"
#include "Renderer.h"

#define OTHER(index) ((index + 1) % 2)

namespace Engine
{
  //-----------------------------------------------------------------------------------------------
  // Render Thread worker function
  //-----------------------------------------------------------------------------------------------
  static void RenderThreadWorker()
  {
    if (Framework::Instance()->GetGraphicsContext()->Init() != Core::ErrorCode::EC_None)
    {
      LOG_ERROR("Unable to set rendering context!");
      RenderThread::Instance()->RenderThreadInitFailed();
      return;
    }

    RendererAPI::Init();
    RT_BindingPoint::Init();
    RenderThreadData::Init();
    RenderThread::Instance()->RenderThreadInitFinished();

    while (!RenderThread::Instance()->ShouldExit())
    {
      Renderer::Instance()->ExecuteRenderCommands();
      RenderThread::Instance()->RenderThreadFrameFinished();
    }
    RenderThreadData::ShutDown();
    RendererAPI::ShutDown();

    if (Framework::Instance()->GetGraphicsContext()->ShutDown() != Core::ErrorCode::EC_None)
      LOG_ERROR("Trouble shutting down the rendering context!!");

    RenderThread::Instance()->RenderThreadShutDownFinished();
  }

  //-----------------------------------------------------------------------------------------------
  // Render Thread
  //-----------------------------------------------------------------------------------------------
  RenderThread * RenderThread::s_instance = nullptr;

  RenderThread::RenderThread()
    : m_index(0)
    , m_returnCode(ReturnCode::None)
    , m_shouldStop(false)
    , m_locks{Locked, Locked}
  {

  }

  bool RenderThread::Init()
  {
    BSR_ASSERT(s_instance == nullptr, "Trying to initialise twice!");
    s_instance = new RenderThread();
    return s_instance->Start();
  }

  void RenderThread::ShutDown()
  {
    s_instance->Stop();
    delete s_instance;
    s_instance = nullptr;
  }

  RenderThread* RenderThread::Instance()
  {
    BSR_ASSERT(s_instance != nullptr, "RenderThread not initialised!");
    return s_instance;
  }

  bool RenderThread::Start()
  {
    m_renderThread = std::thread(RenderThreadWorker);
    {
      std::mutex mutex_start;
      std::unique_lock<std::mutex> lck(mutex_start);
      m_cv.wait(lck, [this]
        {
          return m_returnCode != ReturnCode::None;
        });
    }
    return m_returnCode == ReturnCode::Ready;
  }

  void RenderThread::Stop()
  {
    Sync();
    m_shouldStop = true;
    Continue();

    m_renderThread.join();
  }

  void RenderThread::WaitAndLock(int a_index)
  {
    while (m_locks[a_index] == Locked)
      std::this_thread::yield();
    m_locks[a_index] = Locked;
  }

  void RenderThread::Unlock(int a_index)
  {
    m_locks[a_index] = Unlocked;
  }

  void RenderThread::Sync()
  {
    int renderInd = OTHER(m_index);
    WaitAndLock(renderInd);
  }

  void RenderThread::Continue()
  {
    int mainInd = m_index;
    int renderInd = OTHER(m_index);
    m_index = renderInd;
    Unlock(mainInd);
  }

  void RenderThread::RenderThreadInitFinished()
  {
    int renderInd = OTHER(m_index);
    m_returnCode = ReturnCode::Ready;
    m_cv.notify_all();
  }

  void RenderThread::RenderThreadInitFailed()
  {
    m_returnCode = ReturnCode::Fail;
    m_cv.notify_all();
  }

  void RenderThread::RenderThreadShutDownFinished()
  {

  }

  void RenderThread::RenderThreadFrameFinished()
  {
    int mainInd = m_index;
    int renderInd = OTHER(m_index);
    Unlock(renderInd);
    WaitAndLock(mainInd);
  }

  bool RenderThread::ShouldExit()
  {
    return m_shouldStop;
  }
}