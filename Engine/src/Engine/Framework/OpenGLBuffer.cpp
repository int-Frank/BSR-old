#pragma once

#include <glad/glad.h>
#include "../Buffer.h"
#include "../Renderer.h"
#include  "core_Log.h"

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

    virtual BufferLayout const & GetLayout() const override;
    virtual void SetLayout(BufferLayout const & layout) override;

    virtual uint32_t GetSize() const override;
    virtual RendererID GetRendererID() const override;

  private:

    RendererID m_rendererID;
    uint32_t m_size;
    VertexBufferUsage m_usage;
    BufferLayout m_layout;
  };

  //////////////////////////////////////////////////////////////////////////////////
  // IndexBuffer
  //////////////////////////////////////////////////////////////////////////////////

  class OpenGLIndexBuffer : public IIndexBuffer
  {
  public:
    OpenGLIndexBuffer(void* data, uint32_t size);
    virtual ~OpenGLIndexBuffer();

    virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
    virtual void Bind() const override;

    virtual uint32_t GetCount() const override;

    virtual uint32_t GetSize() const override;
    virtual RendererID GetRendererID() const override;
  private:
    RendererID m_rendererID = 0;
    uint32_t m_size; //buffer size
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
  {
    uint8_t * m_localData = (uint8_t*)malloc(a_size);

    if (m_localData == nullptr)
    {
      LOG_ERROR("Failed to allocate memory in OpenGLVertexBuffer()");
      m_size = 0;
      return;
    }

    memcpy(m_localData, a_data, a_size);

    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferCreate);

    RENDER_SUBMIT(m_rendererID, m_size, m_localData, m_usage, state,
      {
        glCreateBuffers(1, &m_rendererID);
        glNamedBufferData(m_rendererID, m_size, m_localData, OpenGLUsage(m_usage));
        free(m_localData);
        m_localData = nullptr;
      });
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t a_size, VertexBufferUsage a_usage)
    : m_rendererID(0)
    , m_size(a_size)
    , m_usage(a_usage)
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferCreate);

    RENDER_SUBMIT(m_rendererID, m_size, m_usage, state,
      {
        glCreateBuffers(1, &m_rendererID);
        glNamedBufferData(m_rendererID, m_size, nullptr, OpenGLUsage(m_usage));
      });
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferDelete);

    RENDER_SUBMIT(m_rendererID, state,
      {
        glDeleteBuffers(1, &m_rendererID);
      });
  }

  void OpenGLVertexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferSetData);

    RENDER_SUBMIT(m_rendererID, a_offset, a_size, a_data, state,
      {
        glNamedBufferSubData(m_rendererID, a_offset, a_size, a_data);
      });
  }

  void OpenGLVertexBuffer::Bind() const
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferBind);

    RENDER_SUBMIT(m_rendererID, state,
      {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
      });
  }

  BufferLayout const & OpenGLVertexBuffer::GetLayout() const
  {
    return m_layout;
  }

  void OpenGLVertexBuffer::SetLayout(BufferLayout const & a_layout)
  {
    m_layout = a_layout;
  }

  uint32_t OpenGLVertexBuffer::GetSize() const
  {
    return m_size;
  }

  RendererID OpenGLVertexBuffer::GetRendererID() const
  {
    return m_rendererID;
  }

  OpenGLIndexBuffer::OpenGLIndexBuffer(void* a_data, uint32_t a_size)
    : m_rendererID(0)
    , m_size(a_size)
  {
    uint8_t* m_localData = (uint8_t*)malloc(a_size);

    if (m_localData == nullptr)
    {
      LOG_ERROR("Failed to allocate memory in OpenGLVertexBuffer()");
      m_size = 0;
      return;
    }

    memcpy(m_localData, a_data, a_size);

    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferCreate);

    RENDER_SUBMIT(m_rendererID, m_size, m_localData, state,
      {
        glCreateBuffers(1, &m_rendererID);
        glNamedBufferData(m_rendererID, m_size, m_localData, GL_STATIC_DRAW);
        free(m_localData);
        m_localData = nullptr;
      });
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferDelete);

    RENDER_SUBMIT(m_rendererID, state,
      {
        glDeleteBuffers(1, &m_rendererID);
      });
  }

  void OpenGLIndexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferSetData);

    RENDER_SUBMIT(m_rendererID, a_offset, a_size, a_data, state,
      {
        glNamedBufferSubData(m_rendererID, a_offset, a_size, a_data);
      });
  }

  void OpenGLIndexBuffer::Bind() const
  {
    RenderState state;
    state.SetType(RenderState::Type::Command);
    state.SetCommandType(RenderState::CommandType::BufferBind);

    RENDER_SUBMIT(m_rendererID, state,
      {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
      });
  }

  uint32_t OpenGLIndexBuffer::GetCount() const
  {
    return m_size / sizeof(intType);
  }

  uint32_t OpenGLIndexBuffer::GetSize() const
  {
    return m_size;
  }

  uint32_t OpenGLIndexBuffer::GetRendererID() const
  {
    return m_rendererID;
  }
}