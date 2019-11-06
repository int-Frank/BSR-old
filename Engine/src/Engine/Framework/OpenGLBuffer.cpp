#pragma once

#include <glad/glad.h>
#include "../Buffer.h"
#include "../Renderer.h"

namespace Engine 
{

  //////////////////////////////////////////////////////////////////////////////////
  // VertexBuffer
  //////////////////////////////////////////////////////////////////////////////////

  class OpenGLVertexBuffer : public IVertexBuffer
  {
  public:

    OpenGLVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Static);
    OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
    ~OpenGLVertexBuffer();

    virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
    virtual void Bind() const override;

    virtual const BufferLayout& GetLayout() const override;
    virtual void SetLayout(const BufferLayout& layout) override;

    virtual uint32_t GetSize() const override;
    virtual RendererID GetRendererID() const override;

  private:

    RendererID m_rendererID;
    uint32_t m_size;
    VertexBufferUsage m_usage;
    BufferLayout m_layout;

    uint8_t * m_localData;
  };

  //////////////////////////////////////////////////////////////////////////////////
  // IndexBuffer
  //////////////////////////////////////////////////////////////////////////////////

  class OpenGLIndexBuffer : public IIndexBuffer
  {
  public:
    OpenGLIndexBuffer(void* data, uint32_t size);
    virtual ~OpenGLIndexBuffer();

    virtual void SetData(void* data, uint32_t size, uint32_t offset = 0);
    virtual void Bind() const;

    virtual uint32_t GetCount() const;

    virtual uint32_t GetSize() const;
    virtual RendererID GetRendererID() const;
  private:
    RendererID m_rendererID = 0;
    uint32_t m_size; //buffer size

    //Buffer m_LocalData;
  };

  static GLenum OpenGLUsage(VertexBufferUsage usage)
  {
    switch (usage)
    {
      case VertexBufferUsage::Static:    return GL_STATIC_DRAW;
      case VertexBufferUsage::Dynamic:   return GL_DYNAMIC_DRAW;
    }
    BSR_ASSERT(false, "Unknown vertex buffer usage");
    return 0;
  }

  std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(void* a_data, uint32_t a_size, VertexBufferUsage a_usage)
  {
    return std::make_shared<OpenGLVertexBuffer>(a_data, a_size, a_usage);
  }

  std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(uint32_t a_size, VertexBufferUsage a_usage)
  {
    return std::make_shared<OpenGLVertexBuffer>(a_size, a_usage);
  }

  std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(void* a_data, uint32_t a_size)
  {
    return std::make_shared<OpenGLIndexBuffer>(a_data, a_size);
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(void* a_data, uint32_t a_size, VertexBufferUsage a_usage)
    : m_rendererID(0)
    , m_size(a_size)
    , m_usage(a_usage)
    , m_localData(nullptr)
  {
    m_localData = (uint8_t*)realloc(m_localData, a_size);
    memcpy(m_localData, a_data, a_size);

    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::CreateBuffer);

    RENDER_SUBMIT(m_rendererID, m_size, m_localData, m_usage, state,
      {
        glCreateBuffers(1, &m_rendererID);
        glNamedBufferData(m_rendererID, m_size, m_localData, OpenGLUsage(m_usage));
        free(m_localData);
        m_localData = nullptr;
      });
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::DeleteBuffer);

    RENDER_SUBMIT(m_rendererID, state,
      {
        glDeleteBuffers(1, &m_rendererID);
      }
    )
  }
}