//@group Renderer

#pragma once

#include "Buffer.h"
#include "Renderer.h"
#include  "core_Log.h"
#include "RenderThreadData.h"
#include "Serialize.h"

namespace Engine 
{
  //------------------------------------------------------------------------------------------------
  // BufferElement
  //------------------------------------------------------------------------------------------------
  BufferElement::BufferElement(ShaderDataType a_type, std::string const& a_name, bool a_normalized)
    : name(a_name)
    , type(a_type)
    , size(ShaderDataTypeSize(type))
    , offset(0)
    , normalized(a_normalized)
  {

  }

  ShaderDataType ConvertToShaderDataType(uint32_t a_val)
  {
    if (a_val < static_cast<uint32_t>(ShaderDataType::Bool))
      return static_cast<ShaderDataType>(a_val);
    return ShaderDataType::None;
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

  size_t BufferElement::Size() const
  {
    return Core::SerializedSize(name)
      + Core::SerializedSize(type)
      + Core::SerializedSize(size)
      + Core::SerializedSize(offset)
      + Core::SerializedSize(normalized);
  }

  void* BufferElement::Serialize(void* a_out) const
  {
    void * pBuf = a_out;
    uint32_t type32 = static_cast<uint32_t>(type);
    pBuf = Core::Serialize(pBuf, &name);
    pBuf = Core::Serialize(pBuf, &type32);
    pBuf = Core::Serialize(pBuf, &size);
    pBuf = Core::Serialize(pBuf, &offset);
    pBuf = Core::Serialize(pBuf, &normalized);
    return pBuf;
  }

  void const* BufferElement::Deserialize(void const * a_buf)
  {
    void const * pBuf = a_buf;
    uint32_t type32(0);
    pBuf = Core::Deserialize(pBuf, &name);
    pBuf = Core::Deserialize(pBuf, &type32);
    type = ConvertToShaderDataType(type32);
    pBuf = Core::Deserialize(pBuf, &size);
    pBuf = Core::Deserialize(pBuf, &offset);
    pBuf = Core::Deserialize(pBuf, &normalized);
    return pBuf;
  }

  //------------------------------------------------------------------------------------------------
  // BufferLayout
  //------------------------------------------------------------------------------------------------

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
  
  size_t BufferLayout::Size() const
  {
    size_t sze = sizeof(m_stride);
    sze += sizeof(uint32_t);
    for (auto const & item : m_elements)
      sze += item.Size();
    return sze;
  }

  void* BufferLayout::Serialize(void* a_out) const
  {
    uint32_t nElements = static_cast<uint32_t>(m_elements.size());
    void* pBuf = a_out;
    pBuf = Core::Serialize(pBuf, &m_stride);
    pBuf = Core::Serialize(pBuf, &nElements);
    for (auto const& item : m_elements)
      pBuf = item.Serialize(pBuf);
    return pBuf;
  }

  void const* BufferLayout::Deserialize(void const* a_buf)
  {
    uint32_t nElements = 0;
    void const * pBuf = a_buf;
    pBuf = Core::Deserialize(pBuf, &m_stride);
    pBuf = Core::Deserialize(pBuf, &nElements);
    for (uint32_t i = 0; i < nElements; i++)
    {
      BufferElement ele;
      pBuf = ele.Deserialize(pBuf);
      m_elements.push_back(ele);
    }
    return pBuf;
  }

  //------------------------------------------------------------------------------------------------
  // VertexBuffer
  //------------------------------------------------------------------------------------------------
  
  VertexBuffer::VertexBuffer()
  {

  }

  void VertexBuffer::Init(void* a_data, uint32_t a_size, VertexBufferUsage a_usage)
  {
    uint8_t * data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), size = a_size, usage = a_usage, data]()
      {
        ::Engine::RT_VertexBuffer vb;
        vb.Init(data, size, usage);
        ::Engine::RenderThreadData::Instance()->VBOs.insert(resID, vb);
      });
  }

  void VertexBuffer::Init(uint32_t a_size, VertexBufferUsage a_usage)
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), size = a_size, usage = a_usage]()
      {
        ::Engine::RT_VertexBuffer vb;
        vb.Init(size, usage);
        ::Engine::RenderThreadData::Instance()->VBOs.insert(resID, vb);
      });
  }


  Ref<VertexBuffer> VertexBuffer::Create(void* a_data,
                                        uint32_t a_size,
                                        VertexBufferUsage a_usage)
  {
    VertexBuffer * pVBO = new VertexBuffer();
    Ref<VertexBuffer> ref(pVBO); // Need to do it this way to give the object a resource ID
    pVBO->Init(a_data, a_size, a_usage);
    return ref;
  }

  Ref<VertexBuffer> VertexBuffer::Create(uint32_t a_size,
                                         VertexBufferUsage a_usage)
  {
    VertexBuffer* pVBO = new VertexBuffer();
    Ref<VertexBuffer> ref(pVBO); // Need to do it this way to give the object a resource ID
    pVBO->Init(a_size, a_usage);
    return ref;
  }

  VertexBuffer::~VertexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]() mutable
      {
        RT_VertexBuffer * pVB =  RenderThreadData::Instance()->VBOs.at(resID);
        if (pVB == nullptr)
        {
          LOG_WARN("VertexBuffer::~VertexBuffer: RefID '{}' does not exist!", resID);
          return;
        }
        pVB->Destroy();
        ::Engine::RenderThreadData::Instance()->VBOs.erase(resID);
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
        ::Engine::RT_VertexBuffer * pVBO = ::Engine::RenderThreadData::Instance()->VBOs.at(resID);
        if (pVBO == nullptr)
        {
          LOG_WARN("VertexBuffer::SetData(): RefID '{}' does not exist!", resID);
          return;
        }

        pVBO->SetData(data, size, offset);
      });
  }

  void VertexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
      {
        ::Engine::RT_VertexBuffer * pVBO = ::Engine::RenderThreadData::Instance()->VBOs.at(resID);
        if (pVBO == nullptr)
        {
          LOG_WARN("VertexBuffer::Bind(): RefID '{}' does not exist!", resID);
          return;
        }

        pVBO->Bind();
      });
  }

  void VertexBuffer::SetLayout(BufferLayout const& a_layout)
  {
    void * buffer = RENDER_ALLOCATE(a_layout.Size());
    a_layout.Serialize(buffer);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferSetLayout);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), buffer = buffer]()
    {
      ::Engine::RT_VertexBuffer* pVBO = ::Engine::RenderThreadData::Instance()->VBOs.at(resID);
      if (pVBO == nullptr)
      {
        LOG_WARN("VertexBuffer::SetLayout(): RefID '{}' does not exist!", resID);
        return;
      }
      BufferLayout layout;
      layout.Deserialize(buffer);
      pVBO->SetLayout(layout);
    });
  }

  //------------------------------------------------------------------------------------------------
  // IndexBuffer
  //------------------------------------------------------------------------------------------------

  IndexBuffer::IndexBuffer()
  {

  }

  void IndexBuffer::Init(void* a_data, uint32_t a_size)
  {
    uint8_t* data = (uint8_t*)RENDER_ALLOCATE(a_size);
    memcpy(data, a_data, a_size);

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID(), size = a_size, data]()
      {
        ::Engine::RT_IndexBuffer ib;
        ib.Init(data, size);
        ::Engine::RenderThreadData::Instance()->IBOs.insert(resID, ib);
      });
  }

  Ref<IndexBuffer> IndexBuffer::Create(void* a_data, uint32_t a_size)
  {
    IndexBuffer* pIBO = new IndexBuffer();
    Ref<IndexBuffer> ref(pIBO); // Need to do it this way to give the object a resource ID
    pIBO->Init(a_data, a_size);
    return ref;
  }

  IndexBuffer::~IndexBuffer()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferDelete);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]() mutable
      {
        RT_IndexBuffer * pIB =  RenderThreadData::Instance()->IBOs.at(resID);
        if (pIB == nullptr)
        {
          LOG_WARN("IndexBuffer::~IndexBuffer: RefID '{}' does not exist!", resID);
          return;
        }
        pIB->Destroy();
        ::Engine::RenderThreadData::Instance()->IBOs.erase(resID);
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
        ::Engine::RT_IndexBuffer * pIBO = ::Engine::RenderThreadData::Instance()->IBOs.at(resID);
        if (pIBO == nullptr)
        {
          LOG_WARN("IndexBuffer::SetData(): RefID '{}' does not exist!", resID);
          return;
        }

        pIBO->SetData(data, size, offset);
      });
  }

  void IndexBuffer::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::BufferBind);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]()
      {
        ::Engine::RT_IndexBuffer * pIBO = ::Engine::RenderThreadData::Instance()->IBOs.at(resID);
        if (pIBO == nullptr)
        {
          LOG_WARN("IndexBuffer::Bind(): RefID '{}' does not exist!", resID);
          return;
        }

        pIBO->Bind();
      });
  }
}