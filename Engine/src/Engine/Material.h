#ifndef MATERIAL_H
#define MATERIAL_H

#include "Memory.h"
#include "DgMemoryStream.h"
#include "IShader.h"

namespace Engine
{
  class Material
  {
    friend class MaterialInstance;
  public:
    Material(const Ref<Shader>&);
    virtual ~Material();

    void Bind() const;

    template <typename T>
    void Set(StringID uniform, T const &)
    {
      auto decl = FindUniformDeclaration(name);
      // HZ_CORE_ASSERT(decl, "Could not find uniform with name '{0}'", name);
      HZ_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
      auto& buffer = GetUniformBufferTarget(decl);
      buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());

      for (auto mi : m_MaterialInstances)
        mi->OnMaterialValueUpdated(decl);
    }

    void Set(StringID, const Ref<Texture>& texture)
    {
      auto decl = FindResourceDeclaration(name);
      uint32_t slot = decl->GetRegister();
      if (m_Textures.size() <= slot)
        m_Textures.resize((size_t)slot + 1);
      m_Textures[slot] = texture;
    }

    void Set(const std::string& name, const Ref<Texture2D>& texture)
    {
      Set(name, (const Ref<Texture>&)texture);
    }

    void Set(const std::string& name, const Ref<TextureCube>& texture)
    {
      Set(name, (const Ref<Texture>&)texture);
    }
  public:
    static Ref<Material> Create(const Ref<Shader>& shader);
  private:
    void AllocateStorage();
    void OnShaderReloaded();
    void BindTextures() const;

    ShaderUniformDeclaration* FindUniformDeclaration(const std::string& name);
    ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
    Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
  private:
    Ref<Shader> m_Shader;
    std::unordered_set<MaterialInstance*> m_MaterialInstances;

    Dg::MemoryStream m_VSUniformStorageBuffer;
    Dg::MemoryStream m_PSUniformStorageBuffer;
    std::vector<Ref<Texture>> m_Textures;

    int32_t m_RenderFlags = 0;
  };

  class MaterialInstance
  {
    friend class Material;
  public:
    MaterialInstance(const Ref<Material>& material);
    virtual ~MaterialInstance();

    template <typename T>
    void Set(const std::string& name, const T& value)
    {
      auto decl = m_Material->FindUniformDeclaration(name);
      if (!decl)
        return;
      // HZ_CORE_ASSERT(decl, "Could not find uniform with name '{0}'", name);
      HZ_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
      auto& buffer = GetUniformBufferTarget(decl);
      buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());

      m_OverriddenValues.insert(name);
    }

    void Set(const std::string& name, const Ref<Texture>& texture)
    {
      auto decl = m_Material->FindResourceDeclaration(name);
      uint32_t slot = decl->GetRegister();
      if (m_Textures.size() <= slot)
        m_Textures.resize((size_t)slot + 1);
      m_Textures[slot] = texture;
    }

    void Set(const std::string& name, const Ref<Texture2D>& texture)
    {
      Set(name, (const Ref<Texture>&)texture);
    }

    void Set(const std::string& name, const Ref<TextureCube>& texture)
    {
      Set(name, (const Ref<Texture>&)texture);
    }

    void Bind() const;
  public:
    static Ref<MaterialInstance> Create(const Ref<Material>& material);
  private:
    void AllocateStorage();
    void OnShaderReloaded();
    Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
    void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);
  private:
    Ref<Material> m_Material;

    Buffer m_VSUniformStorageBuffer;
    Buffer m_PSUniformStorageBuffer;
    std::vector<Ref<Texture>> m_Textures;

    // TODO: This is temporary; come up with a proper system to track overrides
    std::unordered_set<std::string> m_OverriddenValues;
  };
}

#endif