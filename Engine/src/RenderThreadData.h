//@group Renderer/RenderThread

#ifndef RENDERTHREADDATA_H
#define RENDERTHREADDATA_H

#include "DgOpenHashMap.h"
#include "RendererAPI.h"
#include "RT_Buffer.h"
#include "RT_VertexArray.h"
#include "ResourceID.h"

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

    Dg::OpenHashMap<RefID, ::Engine::RT_VertexBuffer> VBOs;
    Dg::OpenHashMap<RefID, ::Engine::RT_IndexBuffer> IBOs;
    Dg::OpenHashMap<RefID, ::Engine::RT_VertexArray> VAOs;
  };
}

#endif