//@group Renderer

#ifndef RENDERERPROGRAM_H
#define RENDERERPROGRAM_H

#include "Resource.h"
#include "ShaderSource.h"
#include "Memory.h"

namespace Engine
{
  class RendererProgram : public Resource
  {
    void Init();
    RendererProgram();

    RendererProgram(RendererProgram const&) = delete;
    RendererProgram& operator=(RendererProgram const&) = delete;
  public:

    static Ref<RendererProgram> Create();

    ~RendererProgram();

    void Init(ShaderSource const &);
    void Destroy();

    void UploadUniform(std::string const& name, void const * buf, uint32_t size);

    //Use if buf is already on the renderer memory arena
    void UploadUniformNoCopy(std::string const& name, void const * buf, uint32_t size);

    //void Reload(); 
    //void AddShaderReloadedCallback(const ShaderReloadedCallback& callback);

    void Bind();
    void Unbind();

  private:

  };
}

#endif