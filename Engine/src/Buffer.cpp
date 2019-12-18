//@group Renderer

#pragma once

#include <glad/glad.h>
#include "Buffer.h"
#include "Renderer.h"
#include  "core_Log.h"
#include "Resource.h"
#include "MessageBus.h"

namespace Engine 
{
  BufferElement::BufferElement(ShaderDataType a_type, std::string const& a_name, bool a_normalized)
    : name(name)
    , type(a_type)
    , size(ShaderDataTypeSize(type))
    , offset(0)
    , normalized(a_normalized)
  {
  }

  uint32_t ShaderDataTypeSize(ShaderDataType a_type)
  {
    switch (a_type)
    {
      case ShaderDataType::Float:    return 4;
      case ShaderDataType::Float2:   return 4 * 2;
      case ShaderDataType::Float3:   return 4 * 3;
      case ShaderDataType::Float4:   return 4 * 4;
      case ShaderDataType::Mat3:     return 4 * 3 * 3;
      case ShaderDataType::Mat4:     return 4 * 4 * 4;
      case ShaderDataType::Int:      return 4;
      case ShaderDataType::Int2:     return 4 * 2;
      case ShaderDataType::Int3:     return 4 * 3;
      case ShaderDataType::Int4:     return 4 * 4;
      case ShaderDataType::Bool:     return 1;
    }

    BSR_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }

  uint32_t BufferElement::GetComponentCount() const
  {
    switch (type)
    {
      case ShaderDataType::Float:   return 1;
      case ShaderDataType::Float2:  return 2;
      case ShaderDataType::Float3:  return 3;
      case ShaderDataType::Float4:  return 4;
      case ShaderDataType::Mat3:    return 3 * 3;
      case ShaderDataType::Mat4:    return 4 * 4;
      case ShaderDataType::Int:     return 1;
      case ShaderDataType::Int2:    return 2;
      case ShaderDataType::Int3:    return 3;
      case ShaderDataType::Int4:    return 4;
      case ShaderDataType::Bool:    return 1;
    }

    BSR_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }

  BufferLayout::BufferLayout()
    : m_stride(0)
  {

  }

  BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_elements(elements)
    , m_stride(0)
  {
    CalculateOffsetsAndStride();
  }

  uint32_t BufferLayout::GetStride() const
  {
    return m_stride;
  }

  std::vector<BufferElement> const& BufferLayout::GetElements() const
  {
    return m_elements;
  }

  std::vector<BufferElement>::iterator BufferLayout::begin()
  {
    return m_elements.begin();
  }

  std::vector<BufferElement>::iterator BufferLayout::end()
  {
    return m_elements.end();
  }

  std::vector<BufferElement>::const_iterator BufferLayout::begin() const
  {
    return m_elements.begin();
  }

  std::vector<BufferElement>::const_iterator BufferLayout::end() const
  {
    return m_elements.end();
  }

  void BufferLayout::CalculateOffsetsAndStride()
  {
    uint32_t offset = 0;
    m_stride = 0;
    for (auto& element : m_elements)
    {
      element.offset = offset;
      offset += element.size;
      m_stride += element.size;
    }
  }

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

  VertexBuffer::VertexBuffer(void* a_data, uint32_t a_size, VertexBufferUsage a_usage)
    : m_rendererID(0)
    , m_size(a_size)
    , m_usage(a_usage)
  {
    uint8_t * data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    impl::ResourceID64 refID = GetRefID();

    RENDER_SUBMIT(state, [refID = refID, size = m_size, usage = m_usage, data]()
      {
        uint32_t rendererID(0);
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, data, OpenGLUsage(usage));
        auto msg = MessageSub<MT_Command>::New([refID = refID, rendererID = rendererID]()
          {
            Ref<VertexBuffer> ref(refID);
            if (ref.IsNull())
            {
              LOG_WARN("Attempt to update the renderer ID of a VertexBuffer that doesn't exist!");
              return;
            }
            ref->SetRendererID(rendererID);
          });
        POST(msg);
      });
  }

  VertexBuffer::VertexBuffer(uint32_t a_size, VertexBufferUsage a_usage)
    : m_rendererID(0)
    , m_size(a_size)
    , m_usage(a_usage)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    impl::ResourceID64 refID = GetRefID();

    RENDER_SUBMIT(state, [refID = refID, size = m_size, usage = m_usage]()
      {
        uint32_t rendererID(0);
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, nullptr, OpenGLUsage(usage));
        auto msg = MessageSub<MT_Command>::New([refID = refID, rendererID = rendererID]()
          {
            Ref<VertexBuffer> ref(refID);
            if (ref.IsNull())
            {
              LOG_WARN("Attempt to update the renderer ID of a VertexBuffer that doesn't exist!");
              return;
            }
            ref->SetRendererID(rendererID);
          });
        POST(msg);
      });
  }

  VertexBuffer::~VertexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]() mutable
      {
        glDeleteBuffers(1, &rendererID);
      });
  }

  void VertexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    uint8_t* data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferSetData);

    RENDER_SUBMIT(state, [rendererID = m_rendererID, offset = a_offset, size = a_size, data]()
      {
        glNamedBufferSubData(rendererID, offset, size, data);
      });
  }

  void VertexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]()
      {
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
      });
  }

  BufferLayout const & VertexBuffer::GetLayout() const
  {
    return m_layout;
  }

  void VertexBuffer::SetLayout(BufferLayout const & a_layout)
  {
    m_layout = a_layout;
  }

  uint32_t VertexBuffer::GetSize() const
  {
    return m_size;
  }

  RendererID VertexBuffer::GetRendererID() const
  {
    return m_rendererID;
  }

  void VertexBuffer::SetRendererID(RendererID a_id)
  {
    m_rendererID = a_id;
  }

  IndexBuffer::IndexBuffer(void* a_data, uint32_t a_size)
    : m_rendererID(0)
    , m_size(a_size)
  {
    uint8_t* data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    impl::ResourceID64 refID = GetRefID();

    RENDER_SUBMIT(state, [refID = refID, size = m_size, data]()
      {
        uint32_t rendererID(0);
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, data, GL_STATIC_DRAW);
        auto msg = MessageSub<MT_Command>::New([refID = refID, rendererID = rendererID]()
          {
            Ref<IndexBuffer> ref(refID);
            if (ref.IsNull())
            {
              LOG_WARN("Attempt to update the renderer ID of a VertexBuffer that doesn't exist!");
              return;
            }
            ref->SetRendererID(rendererID);
          });
        POST(msg);
      });
  }

  IndexBuffer::~IndexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]() mutable
      {
        glDeleteBuffers(1, &rendererID);
      });
  }

  void IndexBuffer::SetRendererID(RendererID a_id)
  {
    m_rendererID = a_id;
  }

  void IndexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    uint8_t* data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferSetData);

    RENDER_SUBMIT(state, [rendererID = m_rendererID, offset = a_offset, size = a_size, data]()
      {
        glNamedBufferSubData(rendererID, offset, size, data);
      });
  }

  void IndexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]()
      {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
      });
  }

  uint32_t IndexBuffer::GetCount() const
  {
    return m_size / sizeof(intType);
  }

  uint32_t IndexBuffer::GetSize() const
  {
    return m_size;
  }

  uint32_t IndexBuffer::GetRendererID() const
  {
    return m_rendererID;
  }
}