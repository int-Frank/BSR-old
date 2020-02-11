//@group Renderer

#ifndef RENDERERPROGRAM_H
#define RENDERERPROGRAM_H

#include "Resource.h"
#include "ShaderSource.h"

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

    void UploadUniform_bool(std::string const& name, bool);

    //void Reload(); 
    //void AddShaderReloadedCallback(const ShaderReloadedCallback& callback);

    void Bind();
    void Unbind();

  private:

  };
}

#endif