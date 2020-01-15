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

#ifndef SHADERUNIFORM_H
#define SHADERUNIFORM_H

#include <string>
#include <stdint.h>
#include "DgDynamicArray.h"
#include "core_Assert.h"
#include "Memory.h"
#include "Resource.h"
#include "ShaderUtils.h"

#define BIT(x) (1 << x)

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

  //A data type of STRUCT will just be padding. This can be used 
  //to pad out the front and back of a struct.
  class std140ItemDeclaration
  {
  public:

    std140ItemDeclaration(ShaderDataType, uint32_t count, MatrixLayout layout = MatrixLayout::ColumnMajor);
    void * CopyToBuffer(void * buffer, void const * data) const;

    void SetBaseAlignment(uint32_t offset);
    void SetFrontPadding(uint32_t padding);

    ShaderDataType Type() const;

    uint32_t Stride() const;
    uint32_t Count() const;
    uint32_t FrontPadding() const;

  private:

    ShaderDataType  const m_type;  //Cannot be struct
    uint32_t        const m_count;
    MatrixLayout    const m_matLayout;

    uint32_t              m_frontPadding;
  };

  //Matrices will be column-major
  class std140UniformBlock
  {
  public:

    void Push(std140ItemDeclaration const&);
    void * OutputNext(void *);

    uint32_t Size();

  private:
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