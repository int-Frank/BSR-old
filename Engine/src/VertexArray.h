//@group Renderer

#ifndef IVERTEXARRAY_H
#define IVERTEXARRAY_H

#include "Memory.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Resource.h"

namespace Engine {

  class VertexArray : public Resource
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
    void SetRendererID(RendererID);

  private:
    RendererID m_rendererID;
    uint32_t m_vertexAttribIndex;
    std::vector<Ref<VertexBuffer>> m_vertexBuffers;
    Ref<IndexBuffer> m_indexBuffer;
  };
}

#endif