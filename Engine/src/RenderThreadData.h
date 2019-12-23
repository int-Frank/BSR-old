//@group Renderer

#ifndef RENDERTHREADDATA_H
#define RENDERTHREADDATA_H

#include "RendererAPI.h"
#include "DgOpenHashMap.h"

namespace Engine
{
  class RenderThreadData
  {
    static RenderThreadData* s_instance;
  public:

    static bool Init();
    static void ShutDown();
    static RenderThreadData * Instance();

    Dg::OpenHashMap<uint64_t, RendererID> IDMap;
  };
}

#endif