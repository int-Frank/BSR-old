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

#include <string>
#include <stdint.h>
#include "MemBuffer.h"
#include "Memory.h"
#include "core_utils.h"
#include "ShaderUniform.h"
#include "RendererAPI.h"

namespace Engine
{
  class RT_BindingPoint;

  class RT_RendererProgram
  {
  public:

    RT_RendererProgram();
    ~RT_RendererProgram();

    bool InitFromString(std::string const &);
    bool InitFromFilePath(std::string const &);
    void Destroy();

    //void Reload(); 
    //void AddShaderReloadedCallback(const ShaderReloadedCallback& callback);

    void Bind() const;
    void Unbind() const;

    //void SetVSMaterialUniformBuffer(MemBufferDynamic const& buffer);
    //void SetPSMaterialUniformBuffer(MemBufferDynamic const& buffer);

    //void SetFloat(std::string const & name, float);
    //void SetMat4(std::string const& name, const mat4& value);
    //void SetMat4FromRenderThread(std::string const& name, const mat4& value);

    //const std::string& GetName() const;
  private:

    bool Load(std::string const &);
    void SplitSource(std::string const & source);
    void Parse();
    void ParseUniform(std::string const & statement, ShaderDomain);
    void ParseStruct(const std::string& block, ShaderDomain domain);
    bool CompileAndUploadShader();
    void ResolveUniforms();
    void ValidateUniforms();

    //bool Bind(ShaderDomain, std::string const & name, RT_BindingPoint const &);

    ShaderStruct* FindStruct(std::string const & name);
    //int32_t GetUniformLocation(const std::string& name) const;

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
    std::string m_shaderSource[static_cast<uint32_t>(ShaderDomain::COUNT)];
    ShaderUniformDeclarationBuffer * m_uniformBuffers[static_cast<uint32_t>(ShaderDomain::COUNT)];
    ShaderResourceList m_resources;
    ShaderStructList m_structs;

    //std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;

    //ShaderUniformBufferList m_VSRendererUniformBuffers;
    //ShaderUniformBufferList m_PSRendererUniformBuffers;
  };
}

#endif