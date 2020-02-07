//@group Renderer

/*
  Original Copyright Yan Chernikov <github.com/TheCherno/Hazel-dev> and contributors.

  The following code is a derivative work of the code from the GitHub project 'Hazel-dev',
  which is licensed under:

                                  Apache License
                             Version 2.0, January 2004
                          http://www.apache.org/licenses

  This code therefore is also licensed under the terms
  of the Apache License Version 2.0

  Copyright 2017-2019 Frank Hart <frankhart010@gmail.com>
*/

#include "ShaderUniform.h"
#include "Renderer.h"
#include "RT_BindingPoint.h"
#include "RenderThreadData.h"

#include "core_Log.h"
#include "DgMath.h"
#include "core_utils.h"
#include "Serialize.h"

#define ALIGN Dg::ForwardAlign<uint32_t>

namespace Engine
{
  //---------------------------------------------------------------------------------------------------
  // ShaderStruct
  //---------------------------------------------------------------------------------------------------
  ShaderStruct::ShaderStruct(std::string const & a_name)
    : m_name(a_name)
    , m_size(0)
    , m_offset(0)
  {

  }
  
  void ShaderStruct::Log(int a_indent)
  {
    std::string indent(size_t(a_indent) * 2, ' ');
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
  // std140ItemDeclaration
  //---------------------------------------------------------------------------------------------------

  std140ItemDeclaration::std140ItemDeclaration(ShaderDataType a_type,
                                               uint32_t a_count,
                                               MatrixLayout a_layout)
    : m_type(a_type)
    , m_count(a_count)
    , m_matLayout(a_layout)
    , m_frontPadding(0)
  {
    BSR_ASSERT(a_type != ShaderDataType::NONE, "");
  }

  void * std140ItemDeclaration::CopyToBuffer(void * a_buffer, void const * a_data) const
  {
    BSR_ASSERT(m_type != ShaderDataType::NONE, "");

    if (m_count == 0)
      return a_buffer;

    //align first
    void * buf = Core::AdvancePtr(a_buffer, m_frontPadding);

    //copy data
    if (m_type == ShaderDataType::STRUCT)
      return buf; //It's just padding

    uint32_t dataSize = 0;
    uint32_t padding = 0;
    uint32_t count = 0;

    ShaderDataBaseType baseType = GetShaderDataBaseType(m_type);
    uint32_t baseTypeSize = SizeOfShaderDataBaseType(baseType);

    if (GetShaderDataClass(m_type) == ShaderDataClass::Matrix)
    {
      ShaderDataType rowType = GetRowVectorFromMatrix(m_type);
      ShaderDataType columnType = GetColumnVectorFromMatrix(m_type);
      uint32_t nRowElements = GetComponentCount(rowType);
      uint32_t nColumnElements = GetComponentCount(columnType);

      if (m_matLayout == MatrixLayout::RowMajor)
      {
        count = m_count * nColumnElements; 
        dataSize = nRowElements * baseTypeSize;
        uint32_t stride = std140StrideArray(rowType);
        padding = stride - dataSize;
      }
      else
      {
        count = m_count * nRowElements;
        dataSize = nColumnElements * baseTypeSize;
        uint32_t stride = std140StrideArray(columnType);
        padding = stride - dataSize;
      }
    }
    else
    {
      uint32_t nElements = GetComponentCount(m_type);
      dataSize = nElements * baseTypeSize;
      uint32_t stride(0);
      if (m_count == 1)
        stride = std140StrideSingle(m_type);
      else
        stride = std140StrideArray(m_type);
      padding = stride - dataSize;
    }

    byte const * pSrc = static_cast<byte const *>(a_data);
    for (uint32_t c = 0; c < count; c++)
    {
      buf = Core::Serialize<byte>(buf, pSrc, dataSize);
      buf = Core::AdvancePtr(buf, padding);
    }

    return buf;
  }

  uint32_t std140ItemDeclaration::Count() const
  {
    return m_count;
  }

  ShaderDataType std140ItemDeclaration::Type() const
  {
    return m_type;
  }

  void std140ItemDeclaration::SetBaseAlignment(uint32_t a_beginOffset)
  {
    BSR_ASSERT(m_type != ShaderDataType::STRUCT, "Cannot set the base alignment of a struct! Use 'SetPadding()' instead");

    uint32_t alignment(0);
    if (m_count == 1)
      alignment = std140BaseAlignmentSingle(m_type);
    else
      alignment = std140BaseAlignmentSingle(m_type);
    uint32_t offset = ALIGN(a_beginOffset, alignment);
    m_frontPadding = offset - a_beginOffset;
  }

  void std140ItemDeclaration::SetFrontPadding(uint32_t a_padding)
  {
    BSR_ASSERT(m_type == ShaderDataType::STRUCT, "Can only set padding of a STRUCT type!");
    m_frontPadding = a_padding;
  }

  uint32_t std140ItemDeclaration::Stride() const
  {
    if (m_count == 1)
      return std140StrideSingle(m_type);
    return std140StrideArray(m_type);
  }

  uint32_t std140ItemDeclaration::FrontPadding() const
  {
    return m_frontPadding;
  }

  bool operator==(std140ItemDeclaration const& a_item_0, std140ItemDeclaration const& a_item_1)
  {
    bool result = a_item_0.m_type == a_item_1.m_type;
    result &= (a_item_0.m_count == a_item_1.m_count);
    result &= (a_item_0.m_matLayout == a_item_1.m_matLayout);
    return result;
  }

  bool operator!=(std140ItemDeclaration const& a_item_0, std140ItemDeclaration const& a_item_1)
  {
    bool result = a_item_0.m_type == a_item_1.m_type;
    result &= (a_item_0.m_count == a_item_1.m_count);
    result &= (a_item_0.m_matLayout == a_item_1.m_matLayout);
    return !result;
  }

  //---------------------------------------------------------------------------------------------------
  // std140UniformBlock
  //---------------------------------------------------------------------------------------------------

  bool operator==(std140UniformBlock const & a_block_0, std140UniformBlock const & a_block_1)
  {
    if (a_block_0.m_items.size() != a_block_1.m_items.size())
      return false;

    for (uint32_t i = 0; i < uint32_t(a_block_0.m_items.size()); i++)
    {
      if (a_block_0.m_items[i] != a_block_1.m_items[i])
        return false;
    }
    return true;
  }

  //---------------------------------------------------------------------------------------------------
  // std140UniformBlockBuffer
  //---------------------------------------------------------------------------------------------------

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
    std::string indent(size_t(a_indent)  * 2, ' ');
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
    ShaderDataBaseType baseType = GetShaderDataBaseType(m_type);
    uint32_t baseTypeSize = SizeOfShaderDataBaseType(baseType);
    uint32_t nElements = GetComponentCount(m_type);
    m_size = baseTypeSize * nElements * a_count;
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
    std::string indent(size_t(a_indent)  * 2, ' ');
    LOG_DEBUG("{}UNIFORM - name: {}, size: {}, count: {}, offset: {}, domain: {}, type: {}",
      indent.c_str(), m_name.c_str(), m_size, m_count, m_offset, static_cast<uint32_t>(m_domain), ShaderDataTypeToString(m_type).c_str());
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

  void ShaderUniformDeclaration::SetLocation(int32_t a_loc)
  {
    m_location = a_loc;
  }

  void ShaderUniformDeclaration::SetOffset(uint32_t a_offset)
  {
    if (m_type == ShaderDataType::STRUCT)
      m_pStruct->SetOffset(a_offset);

    m_offset = a_offset;
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
    std::string indent(size_t(a_indent)  * 2, ' ');
    LOG_DEBUG("{}UNIFORM DECL BUFFER - name: {}, register: {}, size: {}, domain: {}",
      indent.c_str(), m_name.c_str(), m_register, m_size, static_cast<uint32_t>(m_domain));
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