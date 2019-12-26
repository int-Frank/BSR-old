//@group Renderer/RenderThread

#include <glad/glad.h>
#include "RT_VertexArray.h"
#include "Buffer.h"
#include "RT_Buffer.h"
#include "RenderThreadData.h"

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

  RT_VertexArray::RT_VertexArray()
    : m_rendererID(0)
    , m_indexBuffer(INVALID_REFID)
    , m_vertexAttribIndex(0)
  {

  }

  RT_VertexArray::~RT_VertexArray()
  {

  }

  void RT_VertexArray::Init()
  {
    BSR_ASSERT(m_rendererID == 0, "Already initialized!");
    glCreateVertexArrays(1, &m_rendererID);
  }

  void RT_VertexArray::Destroy()
  {
    BSR_ASSERT(m_rendererID != 0, "Trying to destroy a verex array that wasn't initialized!");
    glDeleteVertexArrays(1, &m_rendererID);
    m_rendererID = 0;
  }

  void RT_VertexArray::Bind() const
  {
    glBindVertexArray(m_rendererID);
  }

  void RT_VertexArray::Unbind() const
  {
    glBindVertexArray(0);
  }

  void RT_VertexArray::AddVertexBuffer(RefID a_id)
  {
    RT_VertexBuffer* pVB = RenderThreadData::Instance()->VBOs.at(a_id);
    if (pVB == nullptr)
    {
      LOG_WARN("RT_VertexArray::AddVertexBuffer(): Failed to find the index buffer! RefID : {}", a_id);
      return;
    }

    BSR_ASSERT(pVB->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
    Bind();
    pVB->Bind();

    for (auto it = pVB->GetLayout().begin(); it != pVB->GetLayout().end(); it++)
    {
      GLenum glBaseType = ShaderDataTypeToOpenGLBaseType(it->type);
      glEnableVertexAttribArray(m_vertexAttribIndex);
      if (glBaseType == GL_INT)
      {
        glVertexAttribIPointer(m_vertexAttribIndex,
          it->GetComponentCount(),
          glBaseType,
          pVB->GetLayout().GetStride(),
          (const void*)(intptr_t)it->offset);
      }
      else
      {
        glVertexAttribPointer(m_vertexAttribIndex,
          it->GetComponentCount(),
          glBaseType,
          it->normalized ? GL_TRUE : GL_FALSE,
          pVB->GetLayout().GetStride(),
          (const void*)(intptr_t)it->offset);
      }
      m_vertexAttribIndex++;
    }
    m_vertexBuffers.push_back(a_id);
  }

  void RT_VertexArray::SetIndexBuffer(RefID a_id)
  {
    RT_IndexBuffer * pIB = RenderThreadData::Instance()->IBOs.at(a_id);
    if (pIB == nullptr)
    {
      LOG_WARN("RT_VertexArray::SetIndexBuffer(): Failed to find the index buffer! RefID : {}", a_id);
      return;
    }

    Bind();
    pIB->Bind();
    m_indexBuffer = a_id;
  }

  Dg::DynamicArray<RefID> const & RT_VertexArray::GetVertexBuffers() const
  {
    return m_vertexBuffers;
  }

  RefID RT_VertexArray::GetIndexBuffer() const
  {
    return m_indexBuffer;
  }
}