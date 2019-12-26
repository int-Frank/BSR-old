//@group Renderer/RenderThread

#ifndef RT_BUFFER
#define RT_BUFFER

#include "Buffer.h"
#include "RendererAPI.h"

namespace Engine
{
  class RT_VertexBuffer
  {
  public:

    RT_VertexBuffer();
    ~RT_VertexBuffer();

    void SetData(void* data, uint32_t size, uint32_t offset = 0);
    void Bind() const;

    void Init(void* data, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
    void Init(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
    void Destroy();

    BufferLayout const& GetLayout() const;
    void SetLayout(BufferLayout const& layout);
    uint32_t GetSize() const;
    RendererID GetRendererID() const;

  private:
    RendererID m_rendererID;
    uint32_t m_size;
    VertexBufferUsage m_usage;
    BufferLayout m_layout;
  };


  class RT_IndexBuffer
  {
  public:
    typedef ::Engine::IndexBuffer::intType intType;

    RT_IndexBuffer();
    ~RT_IndexBuffer();

    void Init(void* data, uint32_t size);
    void Destroy();

    void SetData(void* data, uint32_t size, uint32_t offset = 0);
    void Bind() const;

    uint32_t GetCount() const;
    uint32_t GetSize() const;
    RendererID GetRendererID() const;

  private:
    RendererID m_rendererID;
    uint32_t m_size;
  };
}

#endif
