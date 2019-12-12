 #ifndef RENDERTEMP_H
#define RENDERTEMP_H

#include <stdint.h>
#include "Memory.h"

#include "IShader.h"
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
      auto pStorageBuffer = s_instance->m_commandQueue.Allocate(a_state, renderCmd, sizeof(func));
      new (pStorageBuffer) FuncT(std::forward<FuncT>(func));
    }

    void RenderAndWait();

    //uint64_t RegisterShader(Ref<IShader>);
    //uint64_t RegisterVAO(Ref<IVAO>);
    //uint64_t RegisterTexture(Ref<ITexture>);
    //uint64_t RegisterMaterial(Ref<IMaterial>);

    void * Allocate(size_t);

  private:

    static Renderer * s_instance;

  private:

    Core::MemBuffer<10 * 1024 * 1024> m_memBuffer;
    RenderCommandQueue m_commandQueue;

    Core::Group m_group;

    //Should these belong in an asset manager?
    //std::unique_ptr<ShaderLibrary> m_shaderLibrary;
    //std::unique_ptr<VAOLibrary> m_vaoLibrary;
    //std::unique_ptr<TextureLibrary> m_textureLibrary;

  };

#define RENDER_SUBMIT(...) Renderer::Instance()->Submit(__VA_ARGS__)
#define RENDER_ALLOCATE(size) Renderer::Instance()->Allocate(size)
}

#endif