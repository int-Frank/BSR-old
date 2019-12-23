//@group Renderer

#pragma once

#include <glad/glad.h>
#include "Buffer.h"
#include "Renderer.h"
#include  "core_Log.h"
#include "Resource.h"

namespace Engine 
{
  BufferElement::BufferElement(ShaderDataType a_type, std::string const& a_name, bool a_normalized)
    : name(a_name)
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

  VertexBuffer::VertexBuffer()
    : m_size(0)
    , m_usage(VertexBufferUsage::None)
  {

  }

  void VertexBuffer::Init(void* a_data, uint32_t a_size, VertexBufferUsage a_usage)
  {
    BSR_ASSERT(m_size == 0, "VertexBuffer already initialised!");

    m_size = a_size;
    m_usage = a_usage;

    uint8_t * data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), size = m_size, usage = m_usage, data]()
      {
        RendererID rendererID(0);
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, data, OpenGLUsage(usage));
        RenderThreadData::Instance()->IDMap[resID] = rendererID;
      });
  }

  void VertexBuffer::Init(uint32_t a_size, VertexBufferUsage a_usage)
  {
    BSR_ASSERT(m_size == 0, "VertexBuffer already initialised!");

    m_size = a_size;
    m_usage = a_usage;

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), size = m_size, usage = m_usage]()
      {
        uint32_t rendererID(0);
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, nullptr, OpenGLUsage(usage));
        RenderThreadData::Instance()->IDMap[resID] = rendererID;
      });
  }


  Ref<VertexBuffer> VertexBuffer::Create(void* a_data,
                                        uint32_t a_size,
                                        VertexBufferUsage a_usage)
  {
    VertexBuffer* pVB = new VertexBuffer();
    Ref<VertexBuffer> ref(pVB);
    pVB->Init(a_data, a_size, a_usage);
    return ref;
  }

  Ref<VertexBuffer> VertexBuffer::Create(uint32_t a_size,
                                         VertexBufferUsage a_usage)
  {
    VertexBuffer* pVB = new VertexBuffer();
    Ref<VertexBuffer> ref(pVB);
    pVB->Init(a_size, a_usage);
    return ref;
  }

  VertexBuffer::~VertexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]() mutable
      {
        RendererID * pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("VertexBuffer::~VertexBuffer(): RendererID does not exist! RefID: {}", resID);
          return;
        }

        glDeleteBuffers(1, pID);
        RenderThreadData::Instance()->IDMap.erase(resID);
      });
  }

  void VertexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    uint8_t* data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferSetData);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), offset = a_offset, size = a_size, data]()
      {
        RendererID* pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("VertexBuffer::SetData(): RendererID does not exist! RefID: {}", resID);
          return;
        }

        glNamedBufferSubData(*pID, offset, size, data);
      });
  }

  void VertexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
      {
        RendererID* pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("VertexBuffer::Bind(): RendererID does not exist! RefID: {}", resID);
          return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, *pID);
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

  IndexBuffer::IndexBuffer()
    : m_size(0)
  {

  }

  void IndexBuffer::Init(void* a_data, uint32_t a_size)
  {
    BSR_ASSERT(m_size == 0, "Index buffer already initialised!");

    m_size = a_size;

    uint8_t* data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), size = m_size, data]()
      {
        RendererID rendererID(0);
        glCreateBuffers(1, &rendererID);
        glNamedBufferData(rendererID, size, data, GL_STATIC_DRAW);
        RenderThreadData::Instance()->IDMap[resID] = rendererID;
      });
  }

  Ref<IndexBuffer> IndexBuffer::Create(void* a_data, uint32_t a_size)
  {
    IndexBuffer* pIB = new IndexBuffer();
    Ref<IndexBuffer> ref(pIB);
    pIB->Init(a_data, a_size);
    return ref;
  }

  IndexBuffer::~IndexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]() mutable
      {
        RendererID * pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("IndexBuffer::~IndexBuffer(): RendererID does not exist! RefID: {}", resID);
          return;
        }

        glDeleteBuffers(1, pID);
        RenderThreadData::Instance()->IDMap.erase(resID);
      });
  }

  void IndexBuffer::SetData(void* a_data, uint32_t a_size, uint32_t a_offset)
  {
    uint8_t* data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferSetData);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), offset = a_offset, size = a_size, data]()
      {
        RendererID* pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("IndexBuffer::SetData(): RendererID does not exist! RefID: {}", resID);
          return;
        }

        glNamedBufferSubData(*pID, offset, size, data);
      });
  }

  void IndexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
      {
        RendererID* pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("IndexBuffer::Bind(): RendererID does not exist! RefID: {}", resID);
          return;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pID);
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
}