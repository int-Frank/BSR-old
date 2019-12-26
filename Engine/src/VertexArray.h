//@group Renderer

#ifndef IVERTEXARRAY_H
#define IVERTEXARRAY_H

#include "Memory.h"
#include "Buffer.h"
#include "Resource.h"

namespace Engine 
{
  class VertexArray : public Resource
  {
    VertexArray();
    void Init();
  public:

    static Ref<VertexArray> Create();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(Ref<VertexBuffer> const &);
    void SetIndexBuffer(Ref<IndexBuffer> const &);
  };
}

#endif