//@group Renderer

#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <stdint.h>
#include <string>

namespace Engine
{
  using RendererID = uint32_t;

  struct RenderAPICapabilities
  {
    std::string vendor;
    std::string renderer;
    std::string version;

    int maxSamples;
    float maxAnisotropy;
  };

  class RendererAPI
  {
  private:

  public:
    static void Init();
    static void Shutdown();

    static void Clear(float r, float g, float b, float a);
    static void SetClearColor(float r, float g, float b, float a);

    static void DrawIndexed(unsigned int count, bool depthTest = true);

    static RenderAPICapabilities& GetCapabilities()
    {
      static RenderAPICapabilities capabilities;
      return capabilities;
    }

  };
}

#endif