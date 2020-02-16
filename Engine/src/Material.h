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


#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "Memory.h"
#include "MemBuffer.h"
#include "RendererProgram.h"
#include "core_Assert.h"
#include "DgSet_AVL.h"

namespace Engine
{
  class Material
  {
    void Init(Ref<RendererProgram> &);
    Material();
  public:
    virtual ~Material();

    static Ref<Material> Create(Ref<RendererProgram> &);
    void Bind();

    void SetUniform(std::string const& uniform, void const * data, uint32_t size);

  private:
    Ref<RendererProgram> m_prog;
    //Dg::Set_AVL<MaterialInstance*> m_materialInstances;

    uint32_t m_bufSize;
    byte * m_pBuf;

    //std::vector<Ref<Texture>> m_Textures;

    uint32_t m_renderFlags;
  };

  class MaterialInstance
  {
    friend class Material;
    void Init(Ref<Material> const& material);
    MaterialInstance();
  public:
    static Ref<MaterialInstance> Create(Ref<Material> const& material);
    virtual ~MaterialInstance();

    template <typename T>
    void Set(std::string const & a_uniform, T const & a_value)
    {
      auto decl = m_Material->FindUniformDeclaration(name);
      BSR_ASSERT(decl, "Could not find uniform with name {}", a_uniform.c_str());
      auto& buffer = GetUniformBufferTarget(decl);
      buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());

      m_OverriddenValues.insert(name);
    }

    //void Set(const std::string& name, const Ref<Texture>& texture)
    //{
    //  auto decl = m_Material->FindResourceDeclaration(name);
    //  uint32_t slot = decl->GetRegister();
    //  if (m_Textures.size() <= slot)
    //    m_Textures.resize((size_t)slot + 1);
    //  m_Textures[slot] = texture;
    //}

    //void Set(const std::string& name, const Ref<Texture2D>& texture)
    //{
    //  Set(name, (const Ref<Texture>&)texture);
    //}

    //void Set(const std::string& name, const Ref<TextureCube>& texture)
    //{
    //  Set(name, (const Ref<Texture>&)texture);
    //}

    void Bind() const;
  public:
  private:
    void AllocateStorage();
    void OnShaderReloaded();
    MemBufferDynamic& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
    void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);
  private:
    Ref<Material> m_Material;

    MemBufferDynamic m_VSUniformStorageBuffer;
    MemBufferDynamic m_PSUniformStorageBuffer;
    //std::vector<Ref<Texture>> m_Textures;

    // TODO: This is temporary; come up with a proper system to track overrides
    Dg::Set_AVL<std::string> m_OverriddenValues;
  };
}

#endif