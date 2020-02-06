//@group Renderer

#include "RendererProgram.h"
#include "RT_RendererProgram.h"
#include "Renderer.h"
#include  "core_Log.h"
#include "RenderThreadData.h"

namespace Engine
{
  void RendererProgram::Init()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
    {
      RT_RendererProgram rp;
      RenderThreadData::Instance()->rendererPrograms.insert(resID, rp);
    });
  }

  RendererProgram::RendererProgram()
  {
  
  }

  Ref<RendererProgram> RendererProgram::Create()
  {
    RendererProgram* pRP = new RendererProgram();
    Ref<RendererProgram> ref(pRP); // Need to do it this way to give the object a resource ID
    pRP->Init();
    return ref;
  }

  RendererProgram::~RendererProgram()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramDelete);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
    {
      RT_RendererProgram* pRP = RenderThreadData::Instance()->rendererPrograms.at(resID);
      if (pRP == nullptr)
      {
        LOG_WARN("RendererProgram::~RendererProgram: RefID '{}' does not exist!", resID);
        return;
      }
      pRP->Destroy();
      ::Engine::RenderThreadData::Instance()->rendererPrograms.erase(resID);
    });
  }

  void RendererProgram::Init(ShaderSource const & a_src)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramInit);
    ShaderSource * ptr = new ShaderSource(a_src);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), ptr = ptr]()
    {
      RT_RendererProgram* pRP = RenderThreadData::Instance()->rendererPrograms.at(resID);
      if (pRP == nullptr)
      {
        LOG_WARN("RendererProgram::Init: RefID '{}' does not exist!", resID);
        return;
      }
      pRP->Init(*ptr);
      delete ptr;
    });
  }

  void RendererProgram::Destroy()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramDestroy);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
    {
      RT_RendererProgram* pRP = RenderThreadData::Instance()->rendererPrograms.at(resID);
      if (pRP == nullptr)
      {
        LOG_WARN("RendererProgram::Destroy: RefID '{}' does not exist!", resID);
        return;
      }
      pRP->Destroy();
    });
  }

  void RendererProgram::Bind()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramBind);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
    {
      RT_RendererProgram* pRP = RenderThreadData::Instance()->rendererPrograms.at(resID);
      if (pRP == nullptr)
      {
        LOG_WARN("RendererProgram::Bind: RefID '{}' does not exist!", resID);
        return;
      }
      pRP->Bind();
    });
  }

  void RendererProgram::Unbind()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramUnbind);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
    {
      RT_RendererProgram* pRP = RenderThreadData::Instance()->rendererPrograms.at(resID);
      if (pRP == nullptr)
      {
        LOG_WARN("RendererProgram::Unbind: RefID '{}' does not exist!", resID);
        return;
      }
      pRP->Unbind();
    });
  }
}