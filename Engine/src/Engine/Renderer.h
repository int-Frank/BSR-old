 #ifndef RENDERTEMP_H
#define RENDERTEMP_H

#include <stdint.h>
#include <memory>

#include "IShader.h"
#include "PODArray.h"
#include "DgMap_AVL.h"
#include "RenderState.h"
#include "Group.h"
#include "DgDynamicArray.h"
#include "RenderCommandQueue.h"

namespace Engine
{
  class Renderer
  {
  public:

    typedef int32_t ID;

    static void Init();
    static void ShutDown();
    static Renderer * Instance();

    void BeginScene();

    //Group commands together
    void BeginNewGroup();
    void EndCurrentGroup();

    template<typename FuncT>
    void Submit(RenderState a_state, FuncT&& func)
    {
      static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
      //auto renderCmd = [](void* ptr)
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

    void EndScene();
    void RenderAndWait();

    //uint64_t RegisterShader(std::shared_ptr<IShader>);
    //uint64_t RegisterVAO(std::shared_ptr<IVAO>);
    //uint64_t RegisterTexture(std::shared_ptr<ITexture>);
    //uint64_t RegisterMaterial(std::shared_ptr<IMaterial>);

  private:

    static Renderer * s_instance;

  public:

    RenderCommandQueue m_commandQueue;

    Core::Group m_group;

    //Should these belong in an asset manager?
    //std::unique_ptr<ShaderLibrary> m_shaderLibrary;
    //std::unique_ptr<VAOLibrary> m_vaoLibrary;
    //std::unique_ptr<TextureLibrary> m_textureLibrary;

  };

#define RENDER_SUBMIT(...) Renderer::Instance()->Submit(__VA_ARGS__)
}

#endif