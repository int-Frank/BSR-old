#ifndef EN_ISHADER_H
#define EN_ISHADER_H

#include "MemBuffer.h"
#include "core_utils.h"

namespace Engine
{
  //struct ShaderUniform
  //{

  //};

  //struct ShaderUniformCollection
  //{

  //};

  //enum class UniformType
  //{
  //  None = 0,
  //  Float, Float2, Float3, Float4,
  //  Matrix3x3, Matrix4x4,
  //  Int32, Uint32
  //};

  //struct UniformDecl
  //{
  //  UniformType Type;
  //  std::ptrdiff_t Offset;
  //  std::string Name;
  //};

  //struct UniformBuffer
  //{
  //  // TODO: This currently represents a byte buffer that has been
  //  // packed with uniforms. This was primarily created for OpenGL,
  //  // and needs to be revisted for other rendering APIs. Furthermore,
  //  // this currently does not assume any alignment. This also has
  //  // nothing to do with GL uniform buffers, this is simply a CPU-side
  //  // buffer abstraction.
  //  Core::MemBufferDynamic<> buffer;
  //  std::vector<UniformDecl> uniforms;
  //};

  //struct UniformBufferBase
  //{
  //  virtual byte const * GetBuffer() const = 0;
  //  virtual UniformDecl const * GetUniforms() const = 0;
  //  virtual unsigned int GetUniformCount() const = 0;
  //};

  //template<unsigned int N, unsigned int U>
  //struct UniformBufferDeclaration : public UniformBufferBase
  //{
  //  byte buffer[N];
  //  UniformDecl uniforms[U];
  //  std::ptrdiff_t cursor = 0;
  //  int index = 0;

  //  virtual byte const * GetBuffer() const override
  //  {
  //    return buffer;
  //  }
  //  virtual UniformDecl const * GetUniforms() const override
  //  {
  //    return uniforms;
  //  }
  //  virtual unsigned int GetUniformCount() const
  //  {
  //    return U;
  //  }

  //  template<typename T>
  //  void Push(std::string const & a_name, T const & a_data)
  //  {
  //  }

  //  template<>
  //  void Push(std::string const & a_name, float const & a_data)
  //  {
  //    uniforms[index++] ={UniformType::Float, cursor, a_name};
  //    memcpy(buffer + cursor, &a_data, sizeof(float));
  //    cursor += sizeof(float);
  //  }

  //  template<>
  //  void Push(std::string const & a_name, vec3 const & a_data)
  //  {
  //    uniforms[index++] ={UniformType::Float3, cursor, a_name};
  //    memcpy(buffer + cursor, a_data.GetData(), sizeof(vec3));
  //    cursor += sizeof(vec3);
  //  }

  //  template<>
  //  void Push(std::string const & a_name, vec4 const & a_data)
  //  {
  //    uniforms[index++] ={UniformType::Float4, cursor, a_name};
  //    memcpy(buffer + cursor, a_data.GetData(), sizeof(vec4));
  //    cursor += sizeof(vec4);
  //  }

  //  template<>
  //  void Push(std::string const & a_name, mat4 const & a_data)
  //  {
  //    uniforms[index++] ={UniformType::Matrix4x4, cursor, a_name};
  //    memcpy(buffer + cursor, a_data.GetData(), sizeof(mat4));
  //    cursor += sizeof(mat4);
  //  }

  //};

  class IShader
  {
  public:
    using ShaderReloadedCallback = std::function<void()>;

    virtual void Reload() = 0;

    virtual void Bind() = 0;
    //virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;

    // Temporary while we don't have materials
    //virtual void SetFloat(const std::string& name, float value) = 0;
    //virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
    //virtual void SetMat4FromRenderThread(const std::string& name, const glm::mat4& value) = 0;

    virtual const std::string& GetName() const = 0;

    // Represents a complete shader program stored in a single file.
    // Note: currently for simplicity this is simply a string filepath, however
    //       in the future this will be an asset object + metadata
    //static Ref<IShader> Create(const std::string& filepath);
    //static Ref<IShader> CreateFromString(const std::string& source);

    //virtual void SetVSMaterialUniformBuffer(buffer buffer) = 0;
    //virtual void SetPSMaterialUniformBuffer(buffer buffer) = 0;

    //virtual const ShaderUniformBufferList& GetVSRendererUniforms() const = 0;
    //virtual const ShaderUniformBufferList& GetPSRendererUniforms() const = 0;
    //virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const = 0;
    //virtual const ShaderUniformBufferDeclaration& GetPSMaterialUniformBuffer() const = 0;
    //
    //virtual const ShaderResourceList& GetResources() const = 0;

    virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) = 0;

    // Temporary, before we have an asset manager
    //static std::vector<Ref<Shader>> s_AllShaders;
  };

}

#endif