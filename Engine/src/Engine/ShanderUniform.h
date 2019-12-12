#ifndef SHANDERUNIFORM_H
#define SHANDERUNIFORM_H

#include <string>
#include <stdint.h>
#include "DgDynamicArray.h"
#include "StaticString.h"
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

    ShaderStruct(StringID);

    void AddField(ShaderUniformDeclaration*);
    void SetOffset(uint32_t);
    StringID GetName() const;
    uint32_t GetSize() const;
    uint32_t GetOffset() const;
    Dg::DynamicArray<ShaderUniformDeclaration*> const & GetFields() const;

  private:
    StringID m_name;
    Dg::DynamicArray<ShaderUniformDeclaration*> m_fields;
    uint32_t m_size;
    uint32_t m_offset;
  };

  class ShaderResourceDeclaration
  {
  public:

    enum class Type : uint32_t
    {
      NONE        = 0, 
      TEXTURE2D   = Str::sampler2D, 
      TEXTURECUBE = Str::samplerCube
    };

  public:
    ShaderResourceDeclaration(Type, StringID, uint32_t count);

    StringID GetName() const;
    uint32_t GetRegister() const;
    uint32_t GetCount() const;
    Type GetType() const;

  public:

    static Type StringToType(StringID);
    static StringID TypeToString(Type);

  private:
    StringID m_name;
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
      FLOAT32  = Str::float32, 
      VEC2     = Str::vec2, 
      VEC3     = Str::vec3, 
      VEC4     = Str::vec4, 
      MAT3     = Str::mat3, 
      MAT4     = Str::mat4, 
      INT32    = Str::int32, 
      STRUCT   = Str::udStruct
    };
  public:

    ShaderUniformDeclaration(ShaderDomain, Type, StringID name, uint32_t count = 1);
    ShaderUniformDeclaration(ShaderDomain, ShaderStruct*, StringID name, uint32_t count = 1);

    StringID GetName() const;
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
    static Type StringToType(StringID);
    static StringID TypeToString(Type);

  private:
    StringID m_name;
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
    ShaderUniformBufferDeclaration(StringID name, ShaderDomain);

    void PushUniform(ShaderUniformDeclaration*);

    StringID GetName() const;
    uint32_t GetRegister() const;
    uint32_t GetSize() const;
    ShaderDomain GetDomain() const;
    const ShaderUniformList& GetUniformDeclarations() const;

    ShaderUniformDeclaration* FindUniform(StringID);

  private:
    StringID m_name;
    ShaderUniformList m_uniforms;
    uint32_t m_register;
    uint32_t m_size;
    ShaderDomain m_domain;
  };
}

#endif