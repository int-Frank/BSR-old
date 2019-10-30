#include "Buffer.h"

namespace Engine
{
  BufferElement::BufferElement(ShaderDataType a_type, std::string const & a_name, bool a_normalized)
    : name(name)
    , type(a_type)
    , size(ShaderDataTypeSize(type))
    , offset(0)
    , normalized(normalized)
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
  {
  
  }

  BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_elements(elements)
  {
    CalculateOffsetsAndStride();
  }

  uint32_t BufferLayout::GetStride() const
  {
    return m_stride;
  }

  std::vector<BufferElement> const & BufferLayout::GetElements() const 
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
}