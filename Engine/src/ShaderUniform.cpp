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

#include <regex>

#include "DgStringFunctions.h"

#include "ShaderUniform.h"
#include "Renderer.h"
#include "RT_BindingPoint.h"
#include "RenderThreadData.h"

#include "core_Log.h"
#include "DgMath.h"
#include "core_utils.h"
#include "Serialize.h"
#include "core_Assert.h"

#define ALIGN Dg::ForwardAlign<uint32_t>

//Helpful regex expressions
#define REG_UNIFORM "(?:uniform)"
#define REG_STRUCT "(?:struct)"
#define _OS_ "[\\s\\n\\r]*"
#define _S_ "[\\s\\n\\r]+"
#define VAR "([_a-zA-Z][_a-zA-Z0-9]*)"
#define OARRAY "(?:(?:\\[)" _OS_ "([0-9]+)" _OS_ "(?:\\]))?"
#define SC "[;]"

#define STD140_DECL "(?:layout)" _OS_ "[(]" _OS_ "(?:std140)" _OS_ "[)]"
#define BLOCK_CONTENTS "(?:[^{]*)[{]([^}]*)"

#define UNIFORM_BLOCK_EXPRESSION STD140_DECL _OS_ REG_UNIFORM _S_ VAR BLOCK_CONTENTS

#define VAR_EXPRESSION                            VAR _S_ VAR _OS_ OARRAY _OS_ SC
#define UNIFORM_VAR_EXPRESSION    REG_UNIFORM _S_ VAR _S_ VAR _OS_ OARRAY _OS_ SC
#define STRUCT_EXPRESSION         REG_STRUCT  _S_ VAR BLOCK_CONTENTS

namespace Engine
{
  //---------------------------------------------------------------------------------------------------
  // ShaderStruct
  //---------------------------------------------------------------------------------------------------
  ShaderStruct::ShaderStruct(std::string const & a_name, ShaderDomain a_domain)
    : m_name(a_name)
    , m_domain(a_domain)
  {

  }
  
  void ShaderStruct::Log(int a_indent)
  {
    std::string indent(size_t(a_indent) * 2, ' ');
    LOG_DEBUG("{}STRUCT - name: {}", indent.c_str(), m_name.c_str());
    LOG_DEBUG("{}[", indent.c_str());
    for (auto field : m_fields)
      field->Log(a_indent + 1);
    LOG_DEBUG("{}]", indent.c_str());
  }

  void ShaderStruct::AddField(ShaderUniformDeclaration* a_pField)
  {
    m_fields.push_back(a_pField);
  }

  std::string const & ShaderStruct::GetName() const
  {
    return m_name;
  }

  ShaderDomain ShaderStruct::GetDomain() const
  {
    return m_domain;
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
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderDataType a_type, 
                                                     std::string a_name, 
                                                     uint32_t a_count)
    : m_type(a_type)
    , m_name(a_name)
    , m_count(a_count)
    , m_location(-1)
    , m_pStruct(nullptr)
  {

  }
  
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderStruct* a_pStruct, 
                                                     std::string a_name, 
                                                     uint32_t a_count)
    : m_type(ShaderDataType::STRUCT)
    , m_name(a_name)
    , m_count(a_count)
    , m_location(-1)
    , m_pStruct(a_pStruct)
  {

  }

  void ShaderUniformDeclaration::Log(int a_indent)
  {
    std::string indent(size_t(a_indent)  * 2, ' ');
    LOG_DEBUG("{}UNIFORM - name: {}, count: {}, type: {}, IsVS: {}, IsFS: {}, IsGS: {}",
      indent.c_str(), m_name.c_str(), m_count, ShaderDataTypeToString(m_type).c_str(), 
      m_domains.IsDomain(ShaderDomain::Vertex), 
      m_domains.IsDomain(ShaderDomain::Fragment), 
      m_domains.IsDomain(ShaderDomain::Geometry));
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

  uint32_t ShaderUniformDeclaration::GetCount() const
  {
    return m_count;
  }

  ShaderDomains& ShaderUniformDeclaration::GetDomains()
  {
    return m_domains;
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

  ShaderStruct * ShaderUniformDeclaration::GetShaderUniformStructPtr() const
  {
    return m_pStruct;
  }

  void ShaderUniformDeclaration::SetLocation(int32_t a_loc)
  {
    m_location = a_loc;
  }

  bool operator==(ShaderUniformDeclaration const& a_uniform_0, ShaderUniformDeclaration const& a_uniform_1)
  {
    bool result = a_uniform_0.m_name == a_uniform_1.m_name;
    result = result && (a_uniform_0.m_type == a_uniform_1.m_type);
    result = result && (a_uniform_0.m_count == a_uniform_1.m_count);
    return result;
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderUniformDeclarationBuffer
  //---------------------------------------------------------------------------------------------------
  ShaderUniformDeclarationBuffer::ShaderUniformDeclarationBuffer()
    : m_name("")
    , m_register(0)
  {

  }
  
  ShaderUniformDeclarationBuffer::ShaderUniformDeclarationBuffer(std::string a_name)
    : m_name(a_name)
    , m_register(0)
  {

  }

  void ShaderUniformDeclarationBuffer::Log(int a_indent)
  {
    std::string indent(size_t(a_indent)  * 2, ' ');
    LOG_DEBUG("{}UNIFORM DECL BUFFER - name: {}, register: {}",
      indent.c_str(), m_name.c_str(), m_register);
    for (auto ptr : m_uniforms)
      ptr->Log(a_indent + 1);
    LOG_DEBUG("{}END UNIFORM DECL BUFFER - name: {}", indent.c_str(), m_name.c_str());
  }

  void ShaderUniformDeclarationBuffer::PushUniform(ShaderUniformDeclaration* a_pUniform)
  {
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

  ShaderUniformList& ShaderUniformDeclarationBuffer::GetUniformDeclarations()
  {
    return m_uniforms;
  }

  ShaderUniformList const & ShaderUniformDeclarationBuffer::GetUniformDeclarations() const
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

  void ShaderUniformDeclarationBuffer::Clear()
  {
    for (auto ptr : m_uniforms)
      delete ptr;

    return m_uniforms.clear();
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

  //--------------------------------------------------------------------------------------------------
  // Parsing helper functions
  //--------------------------------------------------------------------------------------------------

  struct varDecl
  {
    std::string type;
    std::string name;
    uint32_t    count;
  };

  typedef Dg::DynamicArray<varDecl> varDeclList;

  static varDeclList __FindDecls(std::string const& a_str, char const* a_regex)
  {
    varDeclList result;
    std::string subject = a_str;

    std::smatch match;
    std::regex r(a_regex);
    while (regex_search(subject, match, r))
    {
      uint32_t count(1);
      if (match.str(3) != "")
        BSR_ASSERT(Dg::StringToNumber<uint32_t>(count, match.str(3), std::dec), "");
      result.push_back(varDecl{match.str(1), match.str(2), count});
      subject = match.suffix().str();
    }
    return result;
  }

  static varDeclList FindDecls(std::string const& a_str)
  {
    return __FindDecls(a_str, VAR_EXPRESSION);
  }

  static varDeclList FindUniformDecls(std::string const& a_str)
  {
    return __FindDecls(a_str, UNIFORM_VAR_EXPRESSION);
  }

  static bool IsTypeStringResource(const std::string& type)
  {
    if (type == "sampler2D")		return true;
    if (type == "samplerCube")		return true;
    if (type == "sampler2DShadow")	return true;
    return false;
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderData
  //---------------------------------------------------------------------------------------------------
  void ShaderData::Load(ShaderSource const & a_src)
  {
    Clear();
    m_pSrc = &a_src;
    Parse();

    uniformBuffer.Log();

    for (auto ptr : structs)
      ptr->Log();
    m_pSrc = nullptr;
  }

  void ShaderData::Clear()
  {
    uniformBuffer.Clear();

    for (auto ptr : resources)
      delete ptr;

    for (auto ptr : structs)
      delete ptr;

    resources.clear();
    structs.clear();
    m_pSrc = nullptr;
  }

  void ShaderData::Parse()
  {
    for (int i = 0; i < ShaderDomain_COUNT; i++)
    {
      ExtractStructs(ShaderDomain(i));
      ExtractUniforms(ShaderDomain(i));
      //ExtractUniformBlocks(ShaderDomain(i));
    }
  }

  void ShaderData::ExtractStructs(ShaderDomain a_domain)
  {
    std::string subject = m_pSrc->Get(a_domain);

    std::smatch match;
    std::regex r(STRUCT_EXPRESSION);
    while (regex_search(subject, match, r))
    {
      ShaderStruct* newStruct = new ShaderStruct(match.str(1), a_domain);
      varDeclList vars = FindDecls(match.str(2));

      for (auto const& var : vars)
      {
        ShaderUniformDeclaration* field = nullptr;
        ShaderDataType dataType = StringToShaderDataType(var.type);
        if (dataType == ShaderDataType::NONE) //might be a previously defined struct
        {
          ShaderStruct* pStruct = FindStruct(var.type, a_domain);
          if (pStruct)
            field = new ShaderUniformDeclaration(pStruct, var.name, var.count);
          else
          {
            LOG_WARN("Unrecognised field '{}' in struct '{}' while parsing glsl struct.", var.type.c_str(), match.str(1).c_str());
            continue;
          }
        }
        else
          field = new ShaderUniformDeclaration(dataType, var.name, var.count);
        newStruct->AddField(field);
      }
      structs.push_back(newStruct);
      subject = match.suffix().str();
    }
  }

  void ShaderData::ExtractUniforms(ShaderDomain a_domain)
  {
    std::string subject = m_pSrc->Get(a_domain);
    varDeclList vars = FindUniformDecls(subject);

    for (auto const& var : vars)
    {
      if (IsTypeStringResource(var.type))
      {
        ShaderResourceDeclaration* pDecl = new ShaderResourceDeclaration(StringToShaderResourceType(var.type), var.name, var.count);
        resources.push_back(pDecl);
      }
      else
      {
        ShaderDataType t = StringToShaderDataType(var.type);
        ShaderUniformDeclaration* pDecl = nullptr;

        if (t == ShaderDataType::NONE)
        {
          ShaderStruct* pStruct = FindStruct(var.type, a_domain);
          BSR_ASSERT(pStruct, "Undefined struct!");
          pDecl = new ShaderUniformDeclaration(pStruct, var.name, var.count);
        }
        else
          pDecl = new ShaderUniformDeclaration(t, var.name, var.count);
        pDecl->GetDomains().AddDomain(a_domain);

        PushUniform(pDecl);
      }
    }
  }

  void ShaderData::PushUniform(ShaderUniformDeclaration* a_pDecl)
  {
    ShaderUniformList& uniforms = uniformBuffer.GetUniformDeclarations();
    for (ShaderUniformDeclaration* pDecl: uniformBuffer.GetUniformDeclarations())
    {
      if (*a_pDecl == *pDecl)
      {
        pDecl->GetDomains().AddDomains(a_pDecl->GetDomains());
        return;
      }
    }
    uniformBuffer.PushUniform(a_pDecl);
  }

  ShaderStruct* ShaderData::FindStruct(std::string const& a_name, ShaderDomain a_domain)
  {
    for (ShaderStruct* ptr : structs)
    {
      if (a_name == ptr->GetName() && a_domain == ptr->GetDomain())
        return ptr;
    }
    return nullptr;
  }

  ShaderUniformDeclaration* ShaderData::FindUniform(std::string const& a_name)
  {
    for (auto pUniform : uniformBuffer.GetUniformDeclarations())
    {
      if (pUniform->GetName() == a_name)
        return pUniform;
    }
    return nullptr;
  }
}