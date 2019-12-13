//@group Renderer

#ifndef IVERTEXARRAY_H
#define IVERTEXARRAY_H

#include "Memory.h"
#include "Buffer.h"
#include "RendererAPI.h"

namespace Engine {

  class VertexArray
  {
  public:

    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

    const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const;
    const Ref<IndexBuffer>& GetIndexBuffer() const;

    static Ref<VertexArray> Create();

  private:
    RendererID m_rendererID = 0;
    uint32_t m_vertexBufferIndex = 0;
    std::vector<Ref<VertexBuffer>> m_vertexBuffers;
    Ref<IndexBuffer> m_indexBuffer;
  };
}

#endif