//@group Renderer

#include "ShaderUniform.h"

namespace Engine
{
  //---------------------------------------------------------------------------------------------------
  // ShanderStruct
  //---------------------------------------------------------------------------------------------------
  ShaderStruct::ShaderStruct(StringID a_name)
    : m_name(a_name)
    , m_size(0)
    , m_offset(0)
  {

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

  StringID ShaderStruct::GetName() const
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
  ShaderResourceDeclaration::ShaderResourceDeclaration(Type a_type, 
                                                       StringID a_name, 
                                                       uint32_t a_count)
    : m_type(a_type)
    , m_name(a_name)
    , m_count(a_count)
    , m_register(0)
  {

  }
  
  StringID ShaderResourceDeclaration::GetName() const
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

  ShaderResourceDeclaration::Type ShaderResourceDeclaration::GetType() const
  {
    return m_type;
  }

  ShaderResourceDeclaration::Type ShaderResourceDeclaration::StringToType(StringID a_str)
  {
    switch (a_str)
    {
      case Str::sampler2D:
      case Str::samplerCube:
        return static_cast<Type>(a_str);
    }

    return Type::NONE;
  }

  StringID ShaderResourceDeclaration::TypeToString(Type a_type)
  {
    switch (a_type)
    {
      case Type::TEXTURE2D:
      case Type::TEXTURECUBE:
        return static_cast<StringID>(a_type);
    }
    return Str::invalidType;
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderUniformDeclaration
  //---------------------------------------------------------------------------------------------------
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderDomain a_domain, 
                                                     Type a_type, 
                                                     StringID a_name, 
                                                     uint32_t a_count)
    : m_domain(a_domain)
    , m_type(a_type)
    , m_name(a_name)
    , m_count(a_count)
    , m_location(-1)
    , m_pStruct(nullptr)
    , m_offset(0)
  {
    m_size = SizeOfUniformType(m_type) * m_count;
  }
  
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderDomain a_domain, 
                                                     ShaderStruct* a_pStruct, 
                                                     StringID a_name, 
                                                     uint32_t a_count)
    : m_domain(a_domain)
    , m_type(Type::STRUCT)
    , m_name(a_name)
    , m_count(a_count)
    , m_location(-1)
    , m_pStruct(a_pStruct)
    , m_offset(0)
  {
    m_size = m_pStruct->GetSize() * m_count;
  }

  StringID ShaderUniformDeclaration::GetName() const
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

  ShaderUniformDeclaration::Type ShaderUniformDeclaration::GetType() const
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

  void ShaderUniformDeclaration::SetOffset(uint32_t offset)
  {
    if (m_type == ShaderUniformDeclaration::Type::STRUCT)
      m_pStruct->SetOffset(offset);

    m_offset = offset;
  }

  uint32_t ShaderUniformDeclaration::SizeOfUniformType(Type a_type)
  {
    switch (a_type)
    {
      case ShaderUniformDeclaration::Type::INT32:      return 4;
      case ShaderUniformDeclaration::Type::FLOAT32:    return 4;
      case ShaderUniformDeclaration::Type::VEC2:       return 4 * 2;
      case ShaderUniformDeclaration::Type::VEC3:       return 4 * 3;
      case ShaderUniformDeclaration::Type::VEC4:       return 4 * 4;
      case ShaderUniformDeclaration::Type::MAT3:       return 4 * 3 * 3;
      case ShaderUniformDeclaration::Type::MAT4:       return 4 * 4 * 4;
    }
    return 0;
  }

  ShaderUniformDeclaration::Type ShaderUniformDeclaration::StringToType(StringID a_type)
  {
    switch (static_cast<Type>(a_type))
    {
      case ShaderUniformDeclaration::Type::INT32:
      case ShaderUniformDeclaration::Type::FLOAT32:
      case ShaderUniformDeclaration::Type::VEC2:
      case ShaderUniformDeclaration::Type::VEC3:
      case ShaderUniformDeclaration::Type::VEC4:
      case ShaderUniformDeclaration::Type::MAT3:
      case ShaderUniformDeclaration::Type::MAT4:
        return static_cast<Type>(a_type);
    }

    return Type::NONE;
  }

  StringID ShaderUniformDeclaration::TypeToString(Type a_type)
  {
    switch (a_type)
    {
      case ShaderUniformDeclaration::Type::INT32:
      case ShaderUniformDeclaration::Type::FLOAT32:
      case ShaderUniformDeclaration::Type::VEC2:
      case ShaderUniformDeclaration::Type::VEC3:
      case ShaderUniformDeclaration::Type::VEC4:
      case ShaderUniformDeclaration::Type::MAT3:
      case ShaderUniformDeclaration::Type::MAT4:
        return static_cast<StringID>(a_type);
    }
    return Str::invalidType;
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderUniformBufferDeclaration
  //---------------------------------------------------------------------------------------------------
  ShaderUniformBufferDeclaration::ShaderUniformBufferDeclaration(StringID a_name, 
                                                                 ShaderDomain a_domain)
    : m_domain(a_domain)
    , m_name(a_name)
    , m_register(0)
    , m_size(0)
  {

  }

  void ShaderUniformBufferDeclaration::PushUniform(ShaderUniformDeclaration* a_pUniform)
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

  StringID ShaderUniformBufferDeclaration::GetName() const
  {
    return m_name;
  }

  uint32_t ShaderUniformBufferDeclaration::GetRegister() const
  {
    return m_register;
  }

  uint32_t ShaderUniformBufferDeclaration::GetSize() const
  {
    return m_size;
  }

  ShaderDomain ShaderUniformBufferDeclaration::GetDomain() const
  {
    return m_domain;
  }

  const ShaderUniformList& ShaderUniformBufferDeclaration::GetUniformDeclarations() const
  {
    return m_uniforms;
  }

  ShaderUniformDeclaration* ShaderUniformBufferDeclaration::FindUniform(StringID a_name)
  {
    for (ShaderUniformDeclaration* uniform : m_uniforms)
    {
      if (uniform->GetName() == a_name)
        return uniform;
    }
    return nullptr;
  }
}