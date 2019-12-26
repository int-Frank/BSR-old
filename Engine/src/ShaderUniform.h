//@group Renderer

#ifndef SHANDERUNIFORM_H
#define SHANDERUNIFORM_H

#include <string>
#include <stdint.h>
#include "DgDynamicArray.h"
#include "core_Assert.h"

namespace Engine
{
  enum class ShaderDomain
  {
    None = 0, Vertex = 0, Pixel = 1
  };

  class ShaderUniformDeclaration;

  class ShaderStruct
  {
  public:

    ShaderStruct(std::string const &);

    void AddField(ShaderUniformDeclaration*);
    void SetOffset(uint32_t);
    std::string const & GetName() const;
    uint32_t GetSize() const;
    uint32_t GetOffset() const;
    Dg::DynamicArray<ShaderUniformDeclaration*> const & GetFields() const;

  private:
    std::string m_name;
    Dg::DynamicArray<ShaderUniformDeclaration*> m_fields;
    uint32_t m_size;
    uint32_t m_offset; //offset in the buffer to where the uniform is kept
  };

  class ShaderResourceDeclaration
  {
  public:

    enum class Type : uint32_t
    {
      NONE        = 0, 
      TEXTURE2D, 
      TEXTURECUBE
    };

  public:
    ShaderResourceDeclaration(Type, std::string const &, uint32_t count);

    std::string const & GetName() const;
    uint32_t GetRegister() const;
    uint32_t GetCount() const;
    Type GetType() const;

  public:

    static Type StringToType(std::string const &);
    static std::string TypeToString(Type);

  private:
    std::string m_name;
    uint32_t m_register;
    uint32_t m_count;
    Type m_type;
  };

  class ShaderUniformDeclaration
  {
    friend class ShaderStruct;
    friend class ShaderUniformBufferDeclaration;
  public:
    enum class Type : uint32_t
    {
      NONE     = 0, 
      FLOAT32, 
      VEC2, 
      VEC3, 
      VEC4, 
      MAT3, 
      MAT4, 
      INT32, 
      STRUCT
    };
  public:

    ShaderUniformDeclaration(ShaderDomain, Type, std::string name, uint32_t count = 1);
    ShaderUniformDeclaration(ShaderDomain, ShaderStruct*, std::string name, uint32_t count = 1);

    std::string GetName() const;
    uint32_t GetSize() const;
    uint32_t GetCount() const;
    uint32_t GetOffset() const;
    uint32_t GetAbsoluteOffset() const;
    ShaderDomain GetDomain() const;
    int32_t GetLocation() const;
    Type GetType() const;
    bool IsArray() const;
    const ShaderStruct& GetShaderUniformStruct() const;

  protected:

    void SetOffset(uint32_t offset);

  public:

    static uint32_t SizeOfUniformType(Type);
    static Type StringToType(std::string);
    static std::string TypeToString(Type);

  private:
    std::string m_name;
    uint32_t m_size;
    uint32_t m_count;
    uint32_t m_offset;
    ShaderDomain m_domain;

    Type m_type;

    ShaderStruct* m_pStruct;
    mutable int32_t m_location;
  };

  typedef Dg::DynamicArray<ShaderUniformDeclaration*> ShaderUniformList;

  class ShaderUniformBufferDeclaration
  {
  public:
    ShaderUniformBufferDeclaration(std::string name, ShaderDomain);

    void PushUniform(ShaderUniformDeclaration*);

    std::string GetName() const;
    uint32_t GetRegister() const;
    uint32_t GetSize() const;
    ShaderDomain GetDomain() const;
    const ShaderUniformList& GetUniformDeclarations() const;

    ShaderUniformDeclaration* FindUniform(std::string);

  private:
    std::string m_name;
    ShaderUniformList m_uniforms;
    uint32_t m_register;
    uint32_t m_size;
    ShaderDomain m_domain;
  };

  typedef Dg::DynamicArray<ShaderUniformBufferDeclaration*> ShaderUniformBufferList;
  typedef Dg::DynamicArray<ShaderResourceDeclaration*> ShaderResourceList;
  typedef Dg::DynamicArray<ShaderStruct*> ShaderStructList;
}

#endif