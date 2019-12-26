////@group Renderer
//
//#ifndef MATERIAL_H
//#define MATERIAL_H
//
//#include <string>
//
//#include "Memory.h"
//#include "MemBuffer.h"
//#include "Shader.h"
//#include "core_Assert.h"
//#include "DgSet_AVL.h"
//
//namespace Engine
//{
//  class Material
//  {
//    friend class MaterialInstance;
//  public:
//    Material(Ref<Shader> const &);
//    virtual ~Material();
//
//    void Bind() const;
//
//    template <typename T>
//    void Set(std::string const & a_uniform, T const & a_value)
//    {
//      auto decl = FindUniformDeclaration(name);
//      BSR_ASSERT(decl, "Could not find uniform with name {}", a_uniform.c_str());
//      auto& buffer = GetUniformBufferTarget(decl);
//      buffer.Write((byte*)&a_value, decl->GetSize(), decl->GetOffset());
//
//      for (auto mi : m_MaterialInstances)
//        mi->OnMaterialValueUpdated(decl);
//    }
//
//    //void Set(std::string, const Ref<Texture>& texture)
//    //{
//    //  auto decl = FindResourceDeclaration(name);
//    //  uint32_t slot = decl->GetRegister();
//    //  if (m_Textures.size() <= slot)
//    //    m_Textures.resize((size_t)slot + 1);
//    //  m_Textures[slot] = texture;
//    //}
//
//    //void Set(const std::string& name, const Ref<Texture2D>& texture)
//    //{
//    //  Set(name, (const Ref<Texture>&)texture);
//    //}
//
//    //void Set(const std::string& name, const Ref<TextureCube>& texture)
//    //{
//    //  Set(name, (const Ref<Texture>&)texture);
//    //}
//  public:
//    static Ref<Material> Create(Ref<Shader> const &);
//  private:
//    void AllocateStorage();
//    void OnShaderReloaded();
//    void BindTextures() const;
//
//    ShaderUniformDeclaration* FindUniformDeclaration(std::string const &);
//    ShaderResourceDeclaration* FindResourceDeclaration(std::string const &);
//    MemBufferDynamic & GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
//  private:
//    Ref<Shader> m_shader;
//    Dg::Set_AVL<MaterialInstance*> m_materialInstances;
//
//    MemBufferDynamic m_VSUniformStorageBuffer;
//    MemBufferDynamic m_PSUniformStorageBuffer;
//    //std::vector<Ref<Texture>> m_Textures;
//
//    uint32_t m_renderFlags = 0;
//  };
//
//  class MaterialInstance
//  {
//    friend class Material;
//  public:
//    MaterialInstance(Ref<Material> const & material);
//    virtual ~MaterialInstance();
//
//    template <typename T>
//    void Set(std::string const & a_uniform, T const & a_value)
//    {
//      auto decl = m_Material->FindUniformDeclaration(name);
//      BSR_ASSERT(decl, "Could not find uniform with name {}", a_uniform.c_str());
//      auto& buffer = GetUniformBufferTarget(decl);
//      buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());
//
//      m_OverriddenValues.insert(name);
//    }
//
//    //void Set(const std::string& name, const Ref<Texture>& texture)
//    //{
//    //  auto decl = m_Material->FindResourceDeclaration(name);
//    //  uint32_t slot = decl->GetRegister();
//    //  if (m_Textures.size() <= slot)
//    //    m_Textures.resize((size_t)slot + 1);
//    //  m_Textures[slot] = texture;
//    //}
//
//    //void Set(const std::string& name, const Ref<Texture2D>& texture)
//    //{
//    //  Set(name, (const Ref<Texture>&)texture);
//    //}
//
//    //void Set(const std::string& name, const Ref<TextureCube>& texture)
//    //{
//    //  Set(name, (const Ref<Texture>&)texture);
//    //}
//
//    void Bind() const;
//  public:
//    static Ref<MaterialInstance> Create(Ref<Material> const & material);
//  private:
//    void AllocateStorage();
//    void OnShaderReloaded();
//    MemBufferDynamic& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
//    void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);
//  private:
//    Ref<Material> m_Material;
//
//    MemBufferDynamic m_VSUniformStorageBuffer;
//    MemBufferDynamic m_PSUniformStorageBuffer;
//    //std::vector<Ref<Texture>> m_Textures;
//
//    // TODO: This is temporary; come up with a proper system to track overrides
//    Dg::Set_AVL<std::string> m_OverriddenValues;
//  };
//}
//
//#endif