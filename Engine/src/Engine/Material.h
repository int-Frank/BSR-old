#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include "IShader.h"

namespace Engine
{
  class Material
  {
    friend class MaterialInstance;
  public:
    Material(std::shared_ptr<IShader> const & shader);
    virtual ~Material();

    void Bind() const;

    template <typename T>
    void Set(std::string const & name, T const & value)
    {
    
    }

    void Set(std::string const & name, std::shared_ptr<Texture> const & texture);

  public:
    static std::shared_ptr<Material> Create(const std::shared_ptr<IShader>& shader);
  private:
    void AllocateStorage();
    void OnShaderReloaded();
    void BindTextures() const;

    ShaderUniformDeclaration* FindUniformDeclaration(const std::string& name);
    ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
    Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
  private:
    std::shared_ptr<Shader> m_Shader;
    std::unordered_set<MaterialInstance*> m_MaterialInstances;

    Buffer m_VSUniformStorageBuffer;
    Buffer m_PSUniformStorageBuffer;
    std::vector<std::shared_ptr<Texture>> m_Textures;

    int32_t m_RenderFlags = 0;
  };
}

#endif