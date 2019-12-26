//@group Renderer/RenderThread

#include "core_Assert.h"
#include "RenderThread.h"
#include "Framework.h"
#include "RendererAPI.h"
#include "RenderThreadData.h"
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
    RenderThreadData::Init();
    RenderThread::Instance()->RenderThreadInitFinished();

    while (!RenderThread::Instance()->ShouldExit())
    {
      LOG_WARN("NEW PASS");
      for (auto it = RenderThreadData::Instance()->VBOs.cbegin();
          it != RenderThreadData::Instance()->VBOs.cend(); it++)
      {
        LOG_DEBUG("RefID: {}, RendererID: {}", it->first, it->second.GetRendererID());
      }

      Renderer::Instance()->ExecuteRenderCommands();
      RenderThread::Instance()->RenderThreadFrameFinished();
    }
    if (Framework::Instance()->GetGraphicsContext()->ShutDown() != Core::ErrorCode::EC_None)
      LOG_ERROR("Trouble shutting down the rendering context!!");
    RenderThreadData::ShutDown();
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
    m_shouldStop = true;
    m_mutex[m_index].unlock();
    m_renderThread.join();
  }

  void RenderThread::Sync()
  {
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
    int renderInd = OTHER(m_index);
    m_mutex[renderInd].unlock();
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