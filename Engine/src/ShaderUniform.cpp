//@group Renderer

#include "ShaderUniform.h"
#include "Renderer.h"
#include "RT_BindingPoint.h"
#include "RenderThreadData.h"

#include "core_Log.h"

namespace Engine
{
  //---------------------------------------------------------------------------------------------------
  // ShanderStruct
  //---------------------------------------------------------------------------------------------------
  ShaderStruct::ShaderStruct(std::string const & a_name)
    : m_name(a_name)
    , m_size(0)
    , m_offset(0)
  {

  }
  
  void ShaderStruct::Log(int a_indent)
  {
    std::string indent(a_indent * 2, ' ');
    LOG_DEBUG("{}STRUCT - name: {}, size: {}, offset: {}", indent.c_str(), m_name.c_str(), m_size, m_offset);
    LOG_DEBUG("{}[", indent.c_str());
    for (auto field : m_fields)
      field->Log(a_indent + 1);
    LOG_DEBUG("{}]", indent.c_str());
  }

  void ShaderStruct::AddField(ShaderUniformDeclaration* a_pField)
  {
    m_size += a_pField->GetSize();
    uint32_t offset = 0;
    if (m_fields.size())
    {
      ShaderUniformDeclaration* previous = m_fields.back();
      offset = previous->GetOffset() + previous->GetSize();
    }
    a_pField->SetOffset(offset);
    m_fields.push_back(a_pField);
  }

  void ShaderStruct::SetOffset(uint32_t a_offset)
  {
    m_offset = a_offset;
  }

  std::string const & ShaderStruct::GetName() const
  {
    return m_name;
  }

  uint32_t ShaderStruct::GetSize() const
  {
    return m_size;
  }

  uint32_t ShaderStruct::GetOffset() const
  {
    return m_offset;
  }

  Dg::DynamicArray<ShaderUniformDeclaration*> const & ShaderStruct::GetFields() const
  {
    return m_fields;
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderResourceDeclaration
  //---------------------------------------------------------------------------------------------------
  ShaderResourceDeclaration::ShaderResourceDeclaration(ShaderResourceType a_type,
                                                       std::string const & a_name, 
                                                       uint32_t a_count)
    : m_type(a_type)
    , m_name(a_name)
    , m_count(a_count)
    , m_register(0)
  {

  }
  void ShaderResourceDeclaration::Log(int a_indent)
  {
    std::string indent(a_indent * 2, ' ');
    LOG_DEBUG("{}RESOURCE - name: {}, count: {}, reg: {}, type: {}", 
      indent.c_str(), m_name.c_str(), m_count, m_register, ShaderResourceTypeToString(m_type).c_str());
  }

  std::string const & ShaderResourceDeclaration::GetName() const
  {
    return m_name;
  }

  uint32_t ShaderResourceDeclaration::GetRegister() const
  {
    return m_register;
  }

  uint32_t ShaderResourceDeclaration::GetCount() const
  {
    return m_count;
  }

  ShaderResourceType ShaderResourceDeclaration::GetType() const
  {
    return m_type;
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderUniformDeclaration
  //---------------------------------------------------------------------------------------------------
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderDomain a_domain, 
                                                     ShaderDataType a_type, 
                                                     std::string a_name, 
                                                     uint32_t a_count)
    : m_domain(a_domain)
    , m_type(a_type)
    , m_name(a_name)
    , m_count(a_count)
    , m_location(-1)
    , m_pStruct(nullptr)
    , m_offset(0)
  {
    m_size = SizeOfShaderDataType(m_type) * m_count;
  }
  
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderDomain a_domain, 
                                                     ShaderStruct* a_pStruct, 
                                                     std::string a_name, 
                                                     uint32_t a_count)
    : m_domain(a_domain)
    , m_type(ShaderDataType::STRUCT)
    , m_name(a_name)
    , m_count(a_count)
    , m_location(-1)
    , m_pStruct(a_pStruct)
    , m_offset(0)
  {
    m_size = m_pStruct->GetSize() * m_count;
  }

  void ShaderUniformDeclaration::Log(int a_indent)
  {
    std::string indent(a_indent * 2, ' ');
    LOG_DEBUG("{}UNIFORM - name: {}, size: {}, count: {}, offset: {}, domain: {}, type: {}",
      indent.c_str(), m_name.c_str(), m_size, m_count, m_offset, m_domain, ShaderDataTypeToString(m_type).c_str());
    if (m_pStruct)
    {
      m_pStruct->Log(a_indent + 1);
      LOG_DEBUG("{}END UNIFORM - name: {}", indent.c_str(), m_name.c_str());
    }
  }

  std::string ShaderUniformDeclaration::GetName() const
  {
    return m_name;
  }

  uint32_t ShaderUniformDeclaration::GetSize() const
  {
    return m_size;
  }

  uint32_t ShaderUniformDeclaration::GetCount() const
  {
    return m_count;
  }

  uint32_t ShaderUniformDeclaration::GetOffset() const
  {
    return m_offset;
  }

  uint32_t ShaderUniformDeclaration::GetAbsoluteOffset() const
  {
    return m_pStruct ? m_pStruct->GetOffset() + m_offset : m_offset;
  }

  ShaderDomain ShaderUniformDeclaration::GetDomain() const
  {
    return m_domain;
  }

  int32_t ShaderUniformDeclaration::GetLocation() const
  {
    return m_location;
  }

  ShaderDataType ShaderUniformDeclaration::GetType() const
  {
    return m_type;
  }

  bool ShaderUniformDeclaration::IsArray() const
  {
    return m_count > 1;
  }

  const ShaderStruct& ShaderUniformDeclaration::GetShaderUniformStruct() const
  {
    BSR_ASSERT(m_pStruct, "");
    return *m_pStruct;
  }

  ShaderStruct * ShaderUniformDeclaration::GetShaderUniformStructPtr() const
  {
    return m_pStruct;
  }

  void ShaderUniformDeclaration::SetOffset(uint32_t offset)
  {
    if (m_type == ShaderDataType::STRUCT)
      m_pStruct->SetOffset(offset);

    m_offset = offset;
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderUniformDeclarationBuffer
  //---------------------------------------------------------------------------------------------------
  ShaderUniformDeclarationBuffer::ShaderUniformDeclarationBuffer(std::string a_name, 
                                                                 ShaderDomain a_domain)
    : m_domain(a_domain)
    , m_name(a_name)
    , m_register(0)
    , m_size(0)
  {

  }

  void ShaderUniformDeclarationBuffer::Log(int a_indent)
  {
    std::string indent(a_indent * 2, ' ');
    LOG_DEBUG("{}UNIFORM DECL BUFFER - name: {}, register: {}, size: {}, domain: {}",
      indent.c_str(), m_name.c_str(), m_register, m_size, m_domain);
    for (auto ptr : m_uniforms)
      ptr->Log(a_indent + 1);
    LOG_DEBUG("{}END UNIFORM DECL BUFFER - name: {}", indent.c_str(), m_name.c_str());
  }

  void ShaderUniformDeclarationBuffer::PushUniform(ShaderUniformDeclaration* a_pUniform)
  {
    uint32_t offset = 0;
    if (m_uniforms.size() > 0)
    {
      ShaderUniformDeclaration* previous = (ShaderUniformDeclaration*)m_uniforms.back();
      offset = previous->GetOffset() + previous->GetSize();
    }
    a_pUniform->SetOffset(offset);
    m_size += a_pUniform->GetSize();
    m_uniforms.push_back(a_pUniform);
  }

  std::string ShaderUniformDeclarationBuffer::GetName() const
  {
    return m_name;
  }

  uint32_t ShaderUniformDeclarationBuffer::GetRegister() const
  {
    return m_register;
  }

  uint32_t ShaderUniformDeclarationBuffer::GetSize() const
  {
    return m_size;
  }

  ShaderDomain ShaderUniformDeclarationBuffer::GetDomain() const
  {
    return m_domain;
  }

  const ShaderUniformList& ShaderUniformDeclarationBuffer::GetUniformDeclarations() const
  {
    return m_uniforms;
  }

  ShaderUniformDeclaration* ShaderUniformDeclarationBuffer::FindUniform(std::string a_name)
  {
    for (ShaderUniformDeclaration* uniform : m_uniforms)
    {
      if (uniform->GetName() == a_name)
        return uniform;
    }
    return nullptr;
  }

  //---------------------------------------------------------------------------------------------------
  // Binding point
  //---------------------------------------------------------------------------------------------------
  BindingPoint::BindingPoint()
  {
  
  }

  Ref<BindingPoint> BindingPoint::Create(StorageBlockType a_type, ShaderDomain a_domain)
  {
    BindingPoint * pBP = new BindingPoint();
    Ref<BindingPoint> ref(pBP);
    pBP->Init(a_type, a_domain);
    return ref;
  }

  void BindingPoint::Init(StorageBlockType a_type, ShaderDomain a_domain)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BindingPointCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), sbtype = a_type, domain = a_domain]()
    {
      ::Engine::RT_BindingPoint bp;
      if (!bp.Capture(sbtype, domain))
        LOG_WARN("BindingPoint::Init(): Failed to capture binding index!");
      ::Engine::RenderThreadData::Instance()->bindingPoints.insert(resID, bp);
    });
  }

  BindingPoint::~BindingPoint()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BindingPointDelete);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
    {
      ::Engine::RT_BindingPoint * pbp = ::Engine::RenderThreadData::Instance()->bindingPoints.at(resID);
      if (pbp == nullptr)
      {
        LOG_WARN("BindingPoint::~BindingPoint(): RefID '{}' does not exist!", resID);
        return;
      }

      pbp->Release();
      ::Engine::RenderThreadData::Instance()->bindingPoints.erase(resID);
    });
  }
}