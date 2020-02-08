//@group Renderer/RenderThread
#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <thread>
#include <atomic>
#include <condition_variable>

namespace Engine
{
  class RenderThread
  {
    enum class ReturnCode
    {
      None,
      Ready,
      Fail,
    };

    static RenderThread * s_instance;

    RenderThread();
    bool Start();
    void Stop();

  public:

    static bool Init();
    static void ShutDown();
    static RenderThread * Instance();

    //Main
    void Sync(); //Sync with the render thread. On return, the render thread will be waiting.
    void Continue(); //Release the render thread after a Sync()

    //Render thread
    void RenderThreadInitFinished();
    void RenderThreadInitFailed();
    void RenderThreadShutDownFinished();
    void RenderThreadFrameFinished();
    bool ShouldExit();

  private:

    void WaitAndLock(int);
    void Unlock(int);

    enum LockState
    {
      Locked,
      Unlocked
    };

    int m_index;
    std::atomic<LockState> m_locks[2];
    std::atomic<ReturnCode> m_returnCode;
    std::condition_variable m_cv;
    std::atomic<bool> m_shouldStop;

    std::thread m_renderThread;
  };
}

#endif