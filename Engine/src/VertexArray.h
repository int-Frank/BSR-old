//@group Renderer

#ifndef IVERTEXARRAY_H
#define IVERTEXARRAY_H

#include "Memory.h"
#include "Buffer.h"
#include "Resource.h"

namespace Engine 
{
  //TODO rename to VAO
  class VertexArray : public Resource
  {
    VertexArray();
    void Init();
  public:

    static Ref<VertexArray> Create();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

    const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const;
    const Ref<IndexBuffer>& GetIndexBuffer() const;

  private:
    uint32_t m_vertexAttribIndex;
    std::vector<Ref<VertexBuffer>> m_vertexBuffers;
    Ref<IndexBuffer> m_indexBuffer;
  };
}

#endif