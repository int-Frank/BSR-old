//@group Renderer/RenderThread

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

#ifndef RT_RENDERERPROGRAM_H
#define RT_RENDERERPROGRAM_H

#include <stdint.h>
#include "MemBuffer.h"
#include "Memory.h"
#include "core_utils.h"
#include "ShaderUniform.h"
#include "RT_RendererAPI.h"
#include "ShaderSource.h"

namespace Engine
{
  class RT_BindingPoint;

  class RT_RendererProgram
  {
  public:

    RT_RendererProgram();
    RT_RendererProgram(impl::ResourceID64);
    ~RT_RendererProgram();

    bool Init(impl::ResourceID64);
    void Destroy();

    //void Reload(); 
    //void AddShaderReloadedCallback(const ShaderReloadedCallback& callback);

    void Bind() const;
    void Unbind() const;

    //void SetVSMaterialUniformBuffer(MemBufferDynamic const& buffer);
    //void SetPSMaterialUniformBuffer(MemBufferDynamic const& buffer);

    void UploadUniform(std::string const& name, void const* data, uint32_t size);

    //const std::string& GetName() const;
  private:

    bool CompileAndUploadShader();
    void ResolveUniforms();

    //bool Bind(ShaderDomain, std::string const & name, RT_BindingPoint const &);

    int32_t GetUniformLocation(std::string const & name) const;

    //void ResolveAndSetUniforms(ShaderUniformDeclarationBuffer* decl, MemBufferDynamic buffer);
    //void ResolveAndSetUniform(ShaderUniformDeclaration* uniform, MemBufferDynamic buffer);
    //void ResolveAndSetUniformArray(ShaderUniformDeclaration* uniform, MemBufferDynamic buffer);
    //void ResolveAndSetUniformField(ShaderUniformDeclaration const& field, byte* data, int32_t offset);
    
    //void UploadUniformInt(uint32_t location, int32_t value);
    //void UploadUniformIntArray(uint32_t location, int32_t* values, int32_t count);
    //void UploadUniformFloat(uint32_t location, float value);
    //void UploadUniformFloat2(uint32_t location, vec2 const& value);
    //void UploadUniformFloat3(uint32_t location, vec3 const& value);
    //void UploadUniformFloat4(uint32_t location, vec4 const& value);
    //void UploadUniformMat3(uint32_t location, mat3 const& values);
    //void UploadUniformMat4(uint32_t location, mat4 const& values);
    //void UploadUniformMat4Array(uint32_t location, mat4& values, uint32_t count);
    
    //void UploadUniformStruct(ShaderUniformDeclaration* uniform, byte* buffer, uint32_t offset);
    
    //void UploadUniformInt(const std::string& name, int32_t value);
    //void UploadUniformIntArray(const std::string& name, int32_t* values, int32_t count);
    
    //void UploadUniformFloat(const std::string& name, float value);
    //void UploadUniformFloat2(const std::string& name, vec2 const& value);
    //void UploadUniformFloat3(const std::string& name, vec3 const& value);
    //void UploadUniformFloat4(const std::string& name, vec4 const& value);
    
    //void UploadUniformMat4(const std::string& name, mat4 const& value);

    //ShaderUniformBufferList const& GetVSRendererUniforms() const;
    //ShaderUniformBufferList const& GetPSRendererUniforms() const;
    //ShaderUniformDeclarationBuffer const& GetVSMaterialUniformBuffer() const;
    //ShaderUniformDeclarationBuffer const& GetPSMaterialUniformBuffer() const;
    //ShaderResourceList const& GetResources() const;
  private:

    RendererID m_rendererID;
    bool m_loaded;

    std::string m_name;
    Ref<ShaderData> m_shaderData; //TODO this needs to be const
    Dg::DynamicArray<int32_t> m_uniformLocations;

    //std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;

  };
}

#endif