//@group Renderer/RenderThread

#include <glad/glad.h>
#include "core_Assert.h"
#include "RT_Buffer.h"
#include "RendererAPI.h"

namespace Engine
{
  static GLenum OpenGLUsage(VertexBufferUsage a_usage)
  {
    switch (a_usage)
    {
      case VertexBufferUsage::Static:    return GL_STATIC_DRAW;
      case VertexBufferUsage::Dynamic:   return GL_DYNAMIC_DRAW;
    }
    BSR_ASSERT(false, "Unknown vertex buffer usage");
    return 0;
  }

  RT_VertexBuffer::RT_VertexBuffer()
    : m_size(0)
    , m_usage(VertexBufferUsage::None)
    , m_rendererID(0)
  {

  }

  void RT_VertexBuffer::Init(void* a_data,
                             uint32_t a_size,
                             VertexBufferUsage a_usage)
  {
    m_size = a_size;
    m_usage = a_usage;

    glCreateBuffers(1, &m_rendererID);
    glNamedBufferData(m_rendererID, m_size, a_data, OpenGLUsage(m_usage));
  }

  void RT_VertexBuffer::Init(uint32_t a_size, VertexBufferUsage a_usage)
  {
    m_size = a_size;
    m_usage = a_usage;

    glCreateBuffers(1, &m_rendererID);
    glNamedBufferData(m_rendererID, m_size, nullptr, OpenGLUsage(m_usage));
  }

  RT_VertexBuffer::~RT_VertexBuffer()
  {

  }

  void RT_VertexBuffer::Destroy()
  {
    glDeleteBuffers(1, &m_rendererID);
    m_rendererID = 0;
  }

  void RT_VertexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    glNamedBufferSubData(m_rendererID, a_offset, a_size, a_data);
  }

  void RT_VertexBuffer::Bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
  }

  BufferLayout const& RT_VertexBuffer::GetLayout() const
  {
    return m_layout;
  }

  void RT_VertexBuffer::SetLayout(BufferLayout const& a_layout)
  {
    m_layout = a_layout;
  }

  uint32_t RT_VertexBuffer::GetSize() const
  {
    return m_size;
  }

  RendererID RT_VertexBuffer::GetRendererID() const
  {
    return m_rendererID;
  }

  //------------------------------------------------------------------------------------------------
  // RT_IndexBuffer
  //------------------------------------------------------------------------------------------------
    
  RT_IndexBuffer::RT_IndexBuffer()
    : m_rendererID(0)
    , m_size(0)
  {
    
  }

  void RT_IndexBuffer::Init(void* a_data, uint32_t a_size)
  {
    m_size = a_size;

    glCreateBuffers(1, &m_rendererID);
    glNamedBufferData(m_rendererID, m_size, a_data, GL_STATIC_DRAW);
  }

  RT_IndexBuffer::~RT_IndexBuffer()
  {

  }

  void RT_IndexBuffer::Destroy()
  {
    glDeleteBuffers(1, &m_rendererID);
    m_rendererID = 0;
  }

  void RT_IndexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    glNamedBufferSubData(m_rendererID, a_offset, a_size, a_data);
  }

  void RT_IndexBuffer::Bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
  }

  uint32_t RT_IndexBuffer::GetCount() const
  {
    return m_size / sizeof(intType);
  }

  uint32_t RT_IndexBuffer::GetSize() const
  {
    return m_size;
  }

  RendererID RT_IndexBuffer::GetRendererID() const
  {
    return m_rendererID;
  }
}