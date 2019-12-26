//@group Renderer

#include "ShaderUniform.h"

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
  ShaderResourceDeclaration::ShaderResourceDeclaration(Type a_type, 
                                                       std::string const & a_name, 
                                                       uint32_t a_count)
    : m_type(a_type)
    , m_name(a_name)
    , m_count(a_count)
    , m_register(0)
  {

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

  ShaderResourceDeclaration::Type ShaderResourceDeclaration::GetType() const
  {
    return m_type;
  }

  ShaderResourceDeclaration::Type ShaderResourceDeclaration::StringToType(std::string const & a_str)
  {
    if (a_str == "sampler2D")
      return Type::TEXTURE2D;
    if (a_str == "samplerCube")
      return Type::TEXTURECUBE;

    return Type::NONE;
  }

  std::string ShaderResourceDeclaration::TypeToString(Type a_type)
  {
    switch (a_type)
    {
      case Type::TEXTURE2D:
        return "sampler2D";
      case Type::TEXTURECUBE:
        return "samplerCube";
    }
    return "None";
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderUniformDeclaration
  //---------------------------------------------------------------------------------------------------
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderDomain a_domain, 
                                                     Type a_type, 
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
    m_size = SizeOfUniformType(m_type) * m_count;
  }
  
  ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderDomain a_domain, 
                                                     ShaderStruct* a_pStruct, 
                                                     std::string a_name, 
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

  ShaderUniformDeclaration::Type ShaderUniformDeclaration::StringToType(std::string a_type)
  {
    //TODO ...
    /*switch (static_cast<Type>(a_type))
    {
      case ShaderUniformDeclaration::Type::INT32:
      case ShaderUniformDeclaration::Type::FLOAT32:
      case ShaderUniformDeclaration::Type::VEC2:
      case ShaderUniformDeclaration::Type::VEC3:
      case ShaderUniformDeclaration::Type::VEC4:
      case ShaderUniformDeclaration::Type::MAT3:
      case ShaderUniformDeclaration::Type::MAT4:
        return static_cast<Type>(a_type);
    }*/

    return Type::NONE;
  }

  std::string ShaderUniformDeclaration::TypeToString(Type a_type)
  {
    //TODO ...
    /*switch (a_type)
    {
      case ShaderUniformDeclaration::Type::INT32:
      case ShaderUniformDeclaration::Type::FLOAT32:
      case ShaderUniformDeclaration::Type::VEC2:
      case ShaderUniformDeclaration::Type::VEC3:
      case ShaderUniformDeclaration::Type::VEC4:
      case ShaderUniformDeclaration::Type::MAT3:
      case ShaderUniformDeclaration::Type::MAT4:
        return static_cast<std::string>(a_type);
    }
    return Str::invalidType;*/
    return "None";
  }

  //---------------------------------------------------------------------------------------------------
  // ShaderUniformBufferDeclaration
  //---------------------------------------------------------------------------------------------------
  ShaderUniformBufferDeclaration::ShaderUniformBufferDeclaration(std::string a_name, 
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

  std::string ShaderUniformBufferDeclaration::GetName() const
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

  ShaderUniformDeclaration* ShaderUniformBufferDeclaration::FindUniform(std::string a_name)
  {
    for (ShaderUniformDeclaration* uniform : m_uniforms)
    {
      if (uniform->GetName() == a_name)
        return uniform;
    }
    return nullptr;
  }
}