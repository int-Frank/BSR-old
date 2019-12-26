//@group Renderer/RenderThread

#ifndef RT_VERTEXARRAY_H
#define RT_VERTEXARRAY_H

#include <stdint.h>
#include "DgDynamicArray.h"
#include "RendererAPI.h"
#include "ResourceID.h"

namespace Engine
{
  class RT_VertexArray
  {
  public:

    RT_VertexArray();
    ~RT_VertexArray();

    void Init();
    void Destroy();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(RefID id);
    void SetIndexBuffer(RefID id);

    Dg::DynamicArray<RefID> const & GetVertexBuffers() const;
    RefID GetIndexBuffer() const;

  private:

    RendererID m_rendererID;
    Dg::DynamicArray<RefID> m_vertexBuffers;
    RefID m_indexBuffer;
    uint32_t m_vertexAttribIndex;
  };
}

#endif