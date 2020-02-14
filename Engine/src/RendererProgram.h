//@group Renderer

#ifndef RENDERERPROGRAM_H
#define RENDERERPROGRAM_H

#include "Resource.h"
#include "ShaderUniform.h"
#include "Memory.h"

namespace Engine
{
  class RendererProgram : public Resource
  {
    void Init(std::initializer_list<ShaderSourceElement> const&);
    RendererProgram();

    RendererProgram(RendererProgram const&) = delete;
    RendererProgram& operator=(RendererProgram const&) = delete;
  public:

    static Ref<RendererProgram> Create(std::initializer_list<ShaderSourceElement> const&);

    ~RendererProgram();

    void Destroy();

    void UploadUniform(std::string const& name, void const * buf, uint32_t size);

    //Use if buf is already on the renderer memory arena
    void UploadUniformNoCopy(std::string const& name, void const * buf, uint32_t size);

    //void Reload(); 
    //void AddShaderReloadedCallback(const ShaderReloadedCallback& callback);

    void Bind();
    void Unbind();

  private:
    //TODO this needs to be Ref<ShaderData const>
    // Now that we have access to the uniform data, we can create a buffer to transform
    // uniforms over to the render thread
    Ref<ShaderData> m_shaderData;
  };
}

#endif