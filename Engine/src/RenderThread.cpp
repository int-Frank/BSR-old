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
    m_mutex[m_index].lock();
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
    //We need to sleep the main thread for a bit to allow the render thread to catch up.
    //Not sure why we need to do this, but I think it has something to do with threads
    //waiting on a mutex can be retasked by the OS and given back to the program
    //at a later time. Even if another thread unlocks the mutex, there is no guarentee
    //the thread will then immediatly take the lock.
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    Sync();
    m_shouldStop = true;
    Continue();

    m_renderThread.join();
    m_mutex[m_index].unlock();
  }

  void RenderThread::Sync()
  {
    //Do not proceed until the render thread has this lock. This may happen in the unlikely
    //case where the render thread has stalled between switching locks, allowing the main
    //thread to complete one full frame.
    while (m_mutex[m_index].try_lock())
      m_mutex[m_index].unlock();

    int renderInd = OTHER(m_index);
    m_mutex[renderInd].lock();
  }

  void RenderThread::Continue()
  {
    int mainInd = m_index;
    int renderInd = OTHER(m_index);
    m_index = renderInd;
    m_mutex[mainInd].unlock();
  }

  void RenderThread::RenderThreadInitFinished()
  {
    int renderInd = OTHER(m_index);
    m_mutex[renderInd].lock();
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
    m_mutex[OTHER(m_index)].unlock();
  }

  void RenderThread::RenderThreadFrameFinished()
  {
    int mainInd = m_index;
    int renderInd = OTHER(m_index);
    m_mutex[renderInd].unlock();
    m_mutex[mainInd].lock();
  }

  bool RenderThread::ShouldExit()
  {
    return m_shouldStop;
  }
}