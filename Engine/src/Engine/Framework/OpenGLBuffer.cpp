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

    virtual void SetData(void* data, uint32_t size, uint32_t offset) override;
    virtual void Bind() const override;

    virtual uint32_t GetCount() const override;

    virtual uint32_t GetSize() const override;
    virtual RendererID GetRendererID() const override;
  private:
    RendererID m_rendererID;
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
    uint8_t * data = (uint8_t*)malloc(a_size);

    if (data == nullptr)
    {
      LOG_ERROR("Failed to allocate memory in OpenGLVertexBuffer()");
      m_size = 0;
      return;
    }

    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [rendererID = m_rendererID, size = m_size, data, usage = m_usage]() mutable
      {
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, data, OpenGLUsage(usage));
        free(data);
      });
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t a_size, VertexBufferUsage a_usage)
    : m_rendererID(0)
    , m_size(a_size)
    , m_usage(a_usage)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [rendererID = m_rendererID, size = m_size, usage = m_usage]() mutable
      {
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, nullptr, OpenGLUsage(usage));
      });
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]() mutable
      {
        glDeleteBuffers(1, &rendererID);
      });
  }

  void OpenGLVertexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferSetData);

    RENDER_SUBMIT(state, [rendererID = m_rendererID, a_offset, a_size, a_data]()
      {
        glNamedBufferSubData(rendererID, a_offset, a_size, a_data);
      });
  }

  void OpenGLVertexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]()
      {
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
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
    uint8_t* data = (uint8_t*)malloc(a_size);

    if (data == nullptr)
    {
      LOG_ERROR("Failed to allocate memory in OpenGLVertexBuffer()");
      m_size = 0;
      return;
    }

    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [rendererID = m_rendererID, size = m_size, data]() mutable
      {
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, data, GL_STATIC_DRAW);
        free(data);
      });
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]() mutable
      {
        glDeleteBuffers(1, &rendererID);
      });
  }

  void OpenGLIndexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferSetData);

    RENDER_SUBMIT(state, [rendererID = m_rendererID, a_offset, a_size, a_data]()
      {
        glNamedBufferSubData(rendererID, a_offset, a_size, a_data);
      });
  }

  void OpenGLIndexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]()
      {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
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