//@group Renderer

#ifndef RENDERTEMP_H
#define RENDERTEMP_H

#include <stdint.h>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>

#include "Memory.h"

#include "Shader.h"
#include "PODArray.h"
#include "DgMap_AVL.h"
#include "RenderState.h"
#include "Group.h"
#include "DgDynamicArray.h"
#include "RenderCommandQueue.h"
#include "MemBuffer.h"

namespace Engine
{
  class Renderer
  {
  public:

    typedef int32_t ID;

    static void Init();
    static void ShutDown();
    static Renderer * Instance();

    Renderer();
    ~Renderer();

    //Everything must happen between these two functions.
    void BeginScene();
    void EndScene();

    //Group commands together
    void BeginNewGroup();
    void EndCurrentGroup();

    template<typename FuncT>
    void Submit(RenderState a_state, FuncT&& func)
    {
      static_assert(std::is_trivially_destructible<FuncT>::value, "FuncT must be trivially destructible");
      RenderCommandFn renderCmd = [](void* ptr)
      {
        auto pFunc = (FuncT*)ptr;
        (*pFunc)();
        pFunc->~FuncT();
      };
      a_state.Set(RenderState::Attr::Group, uint64_t(m_group.GetCurrentID()));
      auto pStorageBuffer = s_instance->m_commandQueue.AllocateForCommand(a_state, renderCmd, sizeof(func));
      new (pStorageBuffer) FuncT(std::forward<FuncT>(func));
    }

    bool ShouldExit() const;

    //Main thread
    void WaitForRenderer();

    //Render thread
    void RenderThreadInit();
    void RenderThreadShutDown();
    void FinishRender();
    void ExecuteRenderCommands();

    void * Allocate(uint32_t);

  private:

    static Renderer * s_instance;

  private:

    RenderCommandQueue m_commandQueue;

    Core::Group m_group;

    std::mutex        m_mutex[2];
    std::mutex        m_mutex3;
    std::atomic<bool> m_shouldExit;
    std::atomic<bool> m_ready;
    std::condition_variable m_cv;

    std::thread m_renderThread;

    int m_trender_ind;
    bool m_trender_hasMutex3;
    int m_tmain_ind;
    bool m_tmain_hasMutex3;
  };

#define RENDER_SUBMIT(...) Renderer::Instance()->Submit(__VA_ARGS__)
#define RENDER_ALLOCATE(size) Renderer::Instance()->Allocate(size)
}

#endif