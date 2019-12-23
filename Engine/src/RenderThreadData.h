#ifndef RENDERTHREADDATA_H
#define RENDERTHREADDATA_H

#include "DgOpenHashMap.h"
#include "RendererAPI.h"

namespace Engine
{
  class RenderThreadData
  {
    static RenderThreadData* s_instance;
  public:

    static bool Init();
    static void ShutDown();
    static RenderThreadData* Instance();

  public:

    Dg::OpenHashMap<uint64_t, RendererID> IDMap;
    Dg::OpenHashMap<uint64_t, uint32_t>   VAOIndex;
  };
}

#endif