//@group Renderer

#include "glad/glad.h"

#include "RenderThreadData.h"
#include "Renderer.h"
#include "RendererAPI.h"
#include "VertexArray.h"
#include "core_Assert.h"
#include "Memory.h"
#include "Resource.h"
#include "Message.h"
#include "MessageBus.h"

namespace Engine
{
  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type)
    {
      case ShaderDataType::Float:    return GL_FLOAT;
      case ShaderDataType::Float2:   return GL_FLOAT;
      case ShaderDataType::Float3:   return GL_FLOAT;
      case ShaderDataType::Float4:   return GL_FLOAT;
      case ShaderDataType::Mat3:     return GL_FLOAT;
      case ShaderDataType::Mat4:     return GL_FLOAT;
      case ShaderDataType::Int:      return GL_INT;
      case ShaderDataType::Int2:     return GL_INT;
      case ShaderDataType::Int3:     return GL_INT;
      case ShaderDataType::Int4:     return GL_INT;
      case ShaderDataType::Bool:     return GL_BOOL;
    }

    BSR_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }

  VertexArray::VertexArray()
    : m_vertexAttribIndex(0)
  {

  }

  void VertexArray::Init()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayCreate);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]() mutable
      {
        RendererID id(0);
        glCreateVertexArrays(1, &id);
        RenderThreadData::Instance()->IDMap[resID] = id;
      });
  }

  Ref<VertexArray> VertexArray::Create()
  {
    VertexArray * p = new VertexArray();
    Ref<VertexArray> ref(p);
    p->Init();
    return ref;
  }

  VertexArray::~VertexArray()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayDelete);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]() mutable
      {
        RendererID * pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("VertexArray::~VertexArray(): RendererID does not exist! RefID: {}", resID);
          return;
        }
        glDeleteVertexArrays(1, pID);
        RenderThreadData::Instance()->IDMap.erase(resID);
      });
  }

  void VertexArray::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayBind);

    RENDER_SUBMIT(state, [resID = GetRefID().GetID()]() mutable
      {
        RendererID* pID =  RenderThreadData::Instance()->IDMap.at(resID);
        if (pID == nullptr)
        {
          LOG_WARN("VertexArray::Bind(): RendererID does not exist! RefID: {}", resID);
          return;
        }
        glBindVertexArray(*pID);
      });
  }

  void VertexArray::Unbind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayUnBind);

    RENDER_SUBMIT(state, []()
      {
        glBindVertexArray(0);
      });
  }

  void VertexArray::AddVertexBuffer(Ref<VertexBuffer> const & a_vertexBuffer)
  {
    BSR_ASSERT(a_vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

    Renderer::Instance()->BeginNewGroup();

    Bind();
    a_vertexBuffer->Bind();

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayAddVertexBuffer);

    RENDER_SUBMIT(state, [vbRefID = a_vertexBuffer->GetRefID(), vertexAttribIndex = m_vertexAttribIndex]() mutable
      {
        Ref<VertexBuffer> vbRef(vbRefID);
        if (vbRef.IsNull())
        {
          LOG_WARN("VertexArray::AddVertexBuffer(): Failed to find the vertex buffer! ");
          return;
        }

        for (auto it = vbRef->GetLayout().begin(); it != vbRef->GetLayout().end(); it++)
        {
          GLenum glBaseType = ShaderDataTypeToOpenGLBaseType(it->type);
          glEnableVertexAttribArray(vertexAttribIndex);
          if (glBaseType == GL_INT)
          {
            glVertexAttribIPointer(vertexAttribIndex,
                                   it->GetComponentCount(),
                                   glBaseType,
                                   vbRef->GetLayout().GetStride(),
                                   (const void*)(intptr_t)it->offset);
          }
          else
          {
            LOG_WARN("ind: {}, comCount: {}, baseType: {}, norm {}, stride: {}, offset: {}",
              vertexAttribIndex,
              it->GetComponentCount(),
              glBaseType,
              it->normalized ? GL_TRUE : GL_FALSE,
              vbRef->GetLayout().GetStride(),
              (const void*)(intptr_t)it->offset);

            glVertexAttribPointer(vertexAttribIndex,
                                  it->GetComponentCount(),
                                  glBaseType,
                                  it->normalized ? GL_TRUE : GL_FALSE,
                                  vbRef->GetLayout().GetStride(),
                                  (const void*)(intptr_t)it->offset);
          }
          vertexAttribIndex++;
        }
      });

    Renderer::Instance()->EndCurrentGroup();
    m_vertexBuffers.push_back(a_vertexBuffer);

    //We just have to trust the above RENDER_SUBMIT succeeds.
    m_vertexAttribIndex += a_vertexBuffer->GetLayout().GetElements().size();
  }

  void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {
    Bind();
    indexBuffer->Bind();

    m_indexBuffer = indexBuffer;
  }
}