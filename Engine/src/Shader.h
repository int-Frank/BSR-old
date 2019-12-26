////@group Renderer
//
//#ifndef EN_SHADER_H
//#define EN_SHADER_H
//
//#include <string>
//#include "MemBuffer.h"
//#include "core_utils.h"
//#include "ShaderUniform.h"
//
//namespace Engine
//{
//  class Shader
//  {
//  public:
//    using ShaderReloadedCallback = std::function<void()>;
//
//    Shader() = default;
//    Shader(const std::string& filepath);
//    static Ref<Shader> CreateFromString(const std::string& source);
//
//    void Reload(); 
//    void AddShaderReloadedCallback(const ShaderReloadedCallback& callback);
//
//    void Bind();
//
//    void SetVSMaterialUniformBuffer(Dg::MemoryStream buffer);
//    void SetPSMaterialUniformBuffer(Dg::MemoryStream buffer);
//
//    void SetFloat(std::string const & name, float);
//    void SetMat4(std::string const& name, const mat4& value);
//    void SetMat4FromRenderThread(StringID name, const mat4& value);
//
//    const std::string& GetName() const
//    {
//      return m_Name;
//    }
//  private:
//
//    void Load(const std::string& source);
//
//    std::string ReadShaderFromFile(const std::string& filepath) const;
//    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
//    void Parse();
//    void ParseUniform(const std::string& statement, ShaderDomain domain);
//    void ParseUniformStruct(const std::string& block, ShaderDomain domain);
//    ShaderStruct* FindStruct(const std::string& name);
//
//    int32_t GetUniformLocation(const std::string& name) const;
//
//    void ResolveUniforms();
//    void ValidateUniforms();
//    void CompileAndUploadShader();
//    static GLenum ShaderTypeFromString(const std::string& type);
//
//    void ResolveAndSetUniforms(ShaderUniformBufferDeclaration * decl, Dg::MemoryStream buffer);
//    void ResolveAndSetUniform(ShaderUniformDeclaration * uniform, Dg::MemoryStream buffer);
//    void ResolveAndSetUniformArray(ShaderUniformDeclaration* uniform, Dg::MemoryStream buffer);
//    void ResolveAndSetUniformField(ShaderUniformDeclaration const & field, byte* data, int32_t offset);
//
//    void UploadUniformInt(uint32_t location, int32_t value);
//    void UploadUniformIntArray(uint32_t location, int32_t* values, int32_t count);
//    void UploadUniformFloat(uint32_t location, float value);
//    void UploadUniformFloat2(uint32_t location, vec2 const & value);
//    void UploadUniformFloat3(uint32_t location, vec3 const & value);
//    void UploadUniformFloat4(uint32_t location, vec4 const& value);
//    void UploadUniformMat3(uint32_t location, mat3 const& values);
//    void UploadUniformMat4(uint32_t location, mat4 const& values);
//    void UploadUniformMat4Array(uint32_t location, mat4& values, uint32_t count);
//
//    void UploadUniformStruct(ShaderUniformDeclaration* uniform, byte* buffer, uint32_t offset);
//
//    void UploadUniformInt(const std::string& name, int32_t value);
//    void UploadUniformIntArray(const std::string& name, int32_t* values, int32_t count);
//
//    void UploadUniformFloat(const std::string& name, float value);
//    void UploadUniformFloat2(const std::string& name, vec2 const& value);
//    void UploadUniformFloat3(const std::string& name, vec3 const& value);
//    void UploadUniformFloat4(const std::string& name, vec4 const& value);
//
//    void UploadUniformMat4(const std::string& name, mat4 const& value);
//
//    ShaderUniformBufferList const & GetVSRendererUniforms() const;
//    ShaderUniformBufferList const & GetPSRendererUniforms() const;
//    ShaderUniformBufferDeclaration const & GetVSMaterialUniformBuffer() const;
//    ShaderUniformBufferDeclaration const & GetPSMaterialUniformBuffer() const;
//    ShaderResourceList const & GetResources() const;
//  private:
//    RendererID m_RendererID = 0; //TODO Put in RenderThreadData
//    bool m_Loaded = false;
//
//    std::string m_Name, m_AssetPath;
//    //std::unordered_map<GLenum, std::string> m_ShaderSource;
//
//    std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
//
//    ShaderUniformBufferList m_VSRendererUniformBuffers;
//    ShaderUniformBufferList m_PSRendererUniformBuffers;
//    ShaderUniformBufferDeclaration * m_VSMaterialUniformBuffer;
//    ShaderUniformBufferDeclaration * m_PSMaterialUniformBuffer;
//    ShaderResourceList m_Resources;
//    ShaderStructList m_Structs;
//  };
//
//}
//
//#endif