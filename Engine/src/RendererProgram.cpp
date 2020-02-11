//@group Renderer

#include "RendererProgram.h"
#include "RT_RendererProgram.h"
#include "Renderer.h"
#include  "core_Log.h"
#include "Serialize.h"
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
      if (!pRP->Init(*ptr))
        LOG_WARN("RendererProgram::Init: Failed! RefID: {}", resID);
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

  void RendererProgram::UploadUniform(std::string const& a_name, void const* a_buf, uint32_t a_size)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramUploadUniform);

    uint32_t sze_name = Core::SerializedSize(a_name);
    void* buf_name = RENDER_ALLOCATE(sze_name);
    Core::Serialize(buf_name, &a_name);

    void* buf_data = RENDER_ALLOCATE(a_size);
    memcpy(buf_data, a_buf, a_size);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), buf_name = buf_name, buf_data = buf_data]()
    {
      RT_RendererProgram* pRP = RenderThreadData::Instance()->rendererPrograms.at(resID);
      if (pRP == nullptr)
      {
        LOG_WARN("RendererProgram::Bind: RefID '{}' does not exist!", resID);
        return;
      }
      std::string name;

      Core::Deserialize(buf_name, &name, 1);
      pRP->UploadUniform(name, buf_data);
    });
  }

  void RendererProgram::UploadUniformNoCopy(std::string const& a_name, void const* a_buf, uint32_t a_size)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::RendererProgramUploadUniform);

    uint32_t sze_name = Core::SerializedSize(a_name);
    void* buf_name = RENDER_ALLOCATE(sze_name);
    Core::Serialize(buf_name, &a_name);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), buf_name = buf_name, buf_data = a_buf]()
    {
      RT_RendererProgram* pRP = RenderThreadData::Instance()->rendererPrograms.at(resID);
      if (pRP == nullptr)
      {
        LOG_WARN("RendererProgram::Bind: RefID '{}' does not exist!", resID);
        return;
      }
      std::string name;

      Core::Deserialize(buf_name, &name, 1);
      pRP->UploadUniform(name, buf_data);
    });
  }
}