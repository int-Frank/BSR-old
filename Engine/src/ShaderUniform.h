//@group Renderer

#ifndef SHANDERUNIFORM_H
#define SHANDERUNIFORM_H

#include <string>
#include <stdint.h>
#include "DgDynamicArray.h"
#include "core_Assert.h"
#include "Memory.h"
#include "Resource.h"
#include "ShaderUtils.h"

namespace Engine
{
  class ShaderUniformDeclaration;

  class ShaderStruct
  {
  public:

    ShaderStruct(std::string const &);

    //DEBUG
    void Log(int a_indent = 0);

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
    ShaderResourceDeclaration(ShaderResourceType, std::string const &, uint32_t count);

    std::string const & GetName() const;
    uint32_t GetRegister() const;
    uint32_t GetCount() const;
    ShaderResourceType GetType() const;

    //DEBUG
    void Log(int a_indent = 0);

    void SetRegister(uint32_t);

  private:
    std::string m_name;
    uint32_t m_register;
    uint32_t m_count;
    ShaderResourceType m_type;
  };

  class ShaderUniformDeclaration
  {
    friend class ShaderStruct;
    friend class ShaderUniformDeclarationBuffer;
  public:

    //DEBUG
    void Log(int a_indent = 0);

    ShaderUniformDeclaration(ShaderDomain, ShaderDataType, std::string name, uint32_t count = 1);
    ShaderUniformDeclaration(ShaderDomain, ShaderStruct*, std::string name, uint32_t count = 1);

    std::string GetName() const;
    uint32_t GetSize() const;
    uint32_t GetCount() const;
    uint32_t GetOffset() const;
    uint32_t GetAbsoluteOffset() const;
    ShaderDomain GetDomain() const;
    int32_t GetLocation() const;
    ShaderDataType GetType() const;
    bool IsArray() const;
    const ShaderStruct& GetShaderUniformStruct() const;
    ShaderStruct * GetShaderUniformStructPtr() const;

    void SetLocation(int32_t);

  protected:

    void SetOffset(uint32_t offset);

  private:
    std::string m_name;
    uint32_t m_size;
    uint32_t m_count;
    uint32_t m_offset;
    ShaderDomain m_domain;

    ShaderDataType m_type;

    ShaderStruct* m_pStruct;
    mutable int32_t m_location; //The OpenGL location
  };

  typedef Dg::DynamicArray<ShaderUniformDeclaration*> ShaderUniformList;

  class ShaderUniformDeclarationBuffer
  {
  public:
    ShaderUniformDeclarationBuffer(std::string name, ShaderDomain);

    void PushUniform(ShaderUniformDeclaration*);

    std::string GetName() const;
    uint32_t GetRegister() const;
    uint32_t GetSize() const;
    ShaderDomain GetDomain() const;
    const ShaderUniformList& GetUniformDeclarations() const;

    //DEBUG
    void Log(int a_indent = 0);

    ShaderUniformDeclaration* FindUniform(std::string);

  private:
    std::string m_name;
    ShaderUniformList m_uniforms;
    uint32_t m_register;
    uint32_t m_size;
    ShaderDomain m_domain;
  };

  typedef Dg::DynamicArray<ShaderUniformDeclarationBuffer*> ShaderUniformBufferList;
  typedef Dg::DynamicArray<ShaderResourceDeclaration*> ShaderResourceList;
  typedef Dg::DynamicArray<ShaderStruct*> ShaderStructList;

  class BindingPoint : public Resource
  {
    void Init(StorageBlockType, ShaderDomain);
    BindingPoint();
  public:

    static Ref<BindingPoint> Create(StorageBlockType, ShaderDomain);

    ~BindingPoint();
  };
}

#endif