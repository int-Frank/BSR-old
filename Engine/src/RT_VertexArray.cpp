//@group Renderer/RenderThread

#include <glad/glad.h>
#include "RT_VertexArray.h"
#include "Buffer.h"
#include "RT_Buffer.h"
#include "RenderThreadData.h"

namespace Engine
{
  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType const a_type)
  {
    switch (a_type)
    {
      case ShaderDataType::FLOAT:
      case ShaderDataType::VEC2:
      case ShaderDataType::VEC3:
      case ShaderDataType::VEC4:
      case ShaderDataType::MAT2:
      case ShaderDataType::MAT3:
      case ShaderDataType::MAT4:
      case ShaderDataType::MAT2x3:
      case ShaderDataType::MAT3x2:
      case ShaderDataType::MAT2x4:
      case ShaderDataType::MAT4x2:
      case ShaderDataType::MAT3x4:
      case ShaderDataType::MAT4x3:
        return GL_FLOAT;
      case ShaderDataType::DOUBLE:
      case ShaderDataType::DVEC2:
      case ShaderDataType::DVEC3:
      case ShaderDataType::DVEC4:
      case ShaderDataType::DMAT2:
      case ShaderDataType::DMAT3:
      case ShaderDataType::DMAT4:
      case ShaderDataType::DMAT2x3:
      case ShaderDataType::DMAT3x2:
      case ShaderDataType::DMAT2x4:
      case ShaderDataType::DMAT4x2:
      case ShaderDataType::DMAT3x4:
      case ShaderDataType::DMAT4x3:
        return GL_DOUBLE;
      case ShaderDataType::INT:
      case ShaderDataType::IVEC2:
      case ShaderDataType::IVEC3:
      case ShaderDataType::IVEC4:
        return GL_INT;
      case ShaderDataType::UINT:
      case ShaderDataType::UVEC2:
      case ShaderDataType::UVEC3:
      case ShaderDataType::UVEC4:
        return GL_UNSIGNED_INT;
      case ShaderDataType::BOOL:
      case ShaderDataType::BVEC2:
      case ShaderDataType::BVEC3:
      case ShaderDataType::BVEC4:
        return GL_BOOL;
    };

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