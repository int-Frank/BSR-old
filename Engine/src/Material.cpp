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


#include "Material.h"
#include "Message.h"
#include "Serialize.h"

namespace Engine
{
  //-----------------------------------------------------------------------------------------------
  // Material
  //-----------------------------------------------------------------------------------------------

  void Material::Init(Ref<RendererProgram> & a_prog)
  {
    if (a_prog.IsNull())
      LOG_WARN("Material created with a null renderer program");
    m_prog = a_prog;
    m_bufSize = a_prog->UniformBufferSize();
    m_pBuf = new byte[m_bufSize];
  }

  Ref<Material> Material::Create(Ref<RendererProgram> & a_prog)
  {
    Material * pmat = new Material();
    Ref<Material> refMat(pmat);
    pmat->Init(a_prog);
    return refMat;
  }

  void Material::Bind()
  {
    if (m_prog.IsNull())
    {
      LOG_WARN("Material::Bind(): Material has a null renderer program");
      return;
    }

    m_prog->Bind();
    m_prog->UploadUniformBuffer(m_pBuf);
  }

  Material::Material()
    : m_pBuf(nullptr)
    , m_bufSize(0)
    , m_renderFlags(0)
  {
    
  }

  Material::~Material()
  {
    delete[] m_pBuf;
  }

  void Material::SetUniform(std::string const& a_name, void const* a_pbuf, uint32_t a_size)
  {
    if (m_prog.IsNull())
    {
      LOG_WARN("Material::Set(): Material has a null renderer program");
      return;
    }

    ShaderUniformDeclaration const * pdecl = m_prog->FindUniformDeclaration(a_name);
    uint32_t count = a_size / SizeOfShaderDataType(pdecl->GetType());

    BSR_ASSERT(pdecl);
    BSR_ASSERT(pdecl->GetCount() >= count);

    uint32_t offset = pdecl->GetDataOffset();
    void * buf = (void*)(m_pBuf + offset);
    buf = Core::Serialize<uint32_t>(buf, &count, 1);
    memcpy(buf, a_pbuf, a_size);
  }

  //ShaderUniformDeclaration* Material::FindUniformDeclaration(const std::string& name)
  //{
  //  if (m_VSUniformStorageBuffer)
  //  {
  //    auto& declarations = m_Shader->GetVSMaterialUniformBuffer().GetUniformDeclarations();
  //    for (ShaderUniformDeclaration* uniform : declarations)
  //    {
  //      if (uniform->GetName() == name)
  //        return uniform;
  //    }
  //  }

  //  if (m_PSUniformStorageBuffer)
  //  {
  //    auto& declarations = m_Shader->GetPSMaterialUniformBuffer().GetUniformDeclarations();
  //    for (ShaderUniformDeclaration* uniform : declarations)
  //    {
  //      if (uniform->GetName() == name)
  //        return uniform;
  //    }
  //  }
  //  return nullptr;
  //}

  //ShaderResourceDeclaration* Material::FindResourceDeclaration(const std::string& name)
  //{
  //  auto& resources = m_Shader->GetResources();
  //  for (ShaderResourceDeclaration* resource : resources)
  //  {
  //    if (resource->GetName() == name)
  //      return resource;
  //  }
  //  return nullptr;
  //}

  //Buffer& Material::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration)
  //{
  //  switch (uniformDeclaration->GetDomain())
  //  {
  //    case ShaderDomain::Vertex:    return m_VSUniformStorageBuffer;
  //    case ShaderDomain::Pixel:     return m_PSUniformStorageBuffer;
  //  }

  //  HZ_CORE_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
  //  return m_VSUniformStorageBuffer;
  //}

  //void Material::Bind() const
  //{
  //  m_Shader->Bind();

  //  if (m_VSUniformStorageBuffer)
  //    m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);

  //  if (m_PSUniformStorageBuffer)
  //    m_Shader->SetPSMaterialUniformBuffer(m_PSUniformStorageBuffer);

  //  BindTextures();
  //}

  //void Material::BindTextures() const
  //{
  //  for (size_t i = 0; i < m_Textures.size(); i++)
  //  {
  //    auto& texture = m_Textures[i];
  //    if (texture)
  //      texture->Bind(i);
  //  }
  //}

  ////////////////////////////////////////////////////////////////////////////////////
  //// MaterialInstance
  ////////////////////////////////////////////////////////////////////////////////////

  //Ref<MaterialInstance> MaterialInstance::Create(const Ref<Material>& material)
  //{
  //  return std::make_shared<MaterialInstance>(material);
  //}

  //MaterialInstance::MaterialInstance(const Ref<Material>& material)
  //  : m_Material(material)
  //{
  //  m_Material->m_MaterialInstances.insert(this);
  //  AllocateStorage();
  //}

  //MaterialInstance::~MaterialInstance()
  //{
  //  m_Material->m_MaterialInstances.erase(this);
  //}

  //void MaterialInstance::OnShaderReloaded()
  //{
  //  AllocateStorage();
  //  m_OverriddenValues.clear();
  //}

  //void MaterialInstance::AllocateStorage()
  //{
  //  const auto& vsBuffer = m_Material->m_Shader->GetVSMaterialUniformBuffer();
  //  m_VSUniformStorageBuffer.Allocate(vsBuffer.GetSize());
  //  memcpy(m_VSUniformStorageBuffer.Data, m_Material->m_VSUniformStorageBuffer.Data, vsBuffer.GetSize());

  //  const auto& psBuffer = m_Material->m_Shader->GetPSMaterialUniformBuffer();
  //  m_PSUniformStorageBuffer.Allocate(psBuffer.GetSize());
  //  memcpy(m_PSUniformStorageBuffer.Data, m_Material->m_PSUniformStorageBuffer.Data, psBuffer.GetSize());
  //}

  //void MaterialInstance::OnMaterialValueUpdated(ShaderUniformDeclaration* decl)
  //{
  //  if (m_OverriddenValues.find(decl->GetName()) == m_OverriddenValues.end())
  //  {
  //    auto& buffer = GetUniformBufferTarget(decl);
  //    auto& materialBuffer = m_Material->GetUniformBufferTarget(decl);
  //    buffer.Write(materialBuffer.Data + decl->GetOffset(), decl->GetSize(), decl->GetOffset());
  //  }
  //}

  //Buffer& MaterialInstance::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration)
  //{
  //  switch (uniformDeclaration->GetDomain())
  //  {
  //    case ShaderDomain::Vertex:    return m_VSUniformStorageBuffer;
  //    case ShaderDomain::Pixel:     return m_PSUniformStorageBuffer;
  //  }

  //  HZ_CORE_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
  //  return m_VSUniformStorageBuffer;
  //}

  //void MaterialInstance::Bind() const
  //{
  //  if (m_VSUniformStorageBuffer)
  //    m_Material->m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);

  //  if (m_PSUniformStorageBuffer)
  //    m_Material->m_Shader->SetPSMaterialUniformBuffer(m_PSUniformStorageBuffer);

  //  m_Material->BindTextures();
  //  for (size_t i = 0; i < m_Textures.size(); i++)
  //  {
  //    auto& texture = m_Textures[i];
  //    if (texture)
  //      texture->Bind(i);
  //  }
  //}
}