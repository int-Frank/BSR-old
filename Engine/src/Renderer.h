//@group Renderer

#ifndef RENDERTEMP_H
#define RENDERTEMP_H

#include <stdint.h>

#include "Memory.h"

#include "PODArray.h"
#include "RendererAPI.h"
#include "RenderState.h"
#include "Group.h"
#include "RenderCommandQueue.h"
#include "MemBuffer.h"
#include "IWindow.h"

#define RENDER_SUBMIT(...) ::Engine::Renderer::Instance()->Submit(__VA_ARGS__)
#define RENDER_ALLOCATE(size) ::Engine::Renderer::Instance()->Allocate(size)

namespace Engine
{
  class Renderer
  {
  public:

    static bool Init();
    static void ShutDown();
    static Renderer * Instance();

    Renderer();
    ~Renderer();

    static void Clear();
    static void Clear(float r, float g, float b, float a = 1.0f);
    static void SetClearColor(float r, float g, float b, float a = 1.0f);

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

    void SwapBuffers();
    void* Allocate(uint32_t);

    //Render thread
    void ExecuteRenderCommands();

  private:

    bool __Init();

  private:

    static Renderer * s_instance;

  private:

    enum class ReturnCode
    {
      None,
      Ready,
      Fail,
    };

    RenderCommandQueue m_commandQueue;
    Core::Group m_group;
  };

}

#endif