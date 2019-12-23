//@group Renderer

#ifndef EN_BUFFER_H
#define EN_BUFFER_H

#include "Memory.h"
#include <stdint.h>
#include <string>
#include <vector>

#include "core_Assert.h"
#include "Resource.h"

namespace Engine 
{
  enum class ShaderDataType
  {
    None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
  };

  uint32_t ShaderDataTypeSize(ShaderDataType a_type);

  struct BufferElement
  {
    std::string name;
    ShaderDataType type;
    uint32_t size;
    uint32_t offset;
    bool normalized;

    BufferElement() = default;

    BufferElement(ShaderDataType, std::string const & name, bool normalized = false);
    uint32_t GetComponentCount() const;
  };

  class BufferLayout
  {
  public:
    BufferLayout();

    BufferLayout(const std::initializer_list<BufferElement>& elements);

    uint32_t GetStride() const;
    std::vector<BufferElement> const & GetElements() const;

    std::vector<BufferElement>::iterator begin();
    std::vector<BufferElement>::iterator end();
    std::vector<BufferElement>::const_iterator begin() const;
    std::vector<BufferElement>::const_iterator end() const;

  private:

    void CalculateOffsetsAndStride();

  private:
    //TODO Make this a Dg::DynamicArray, only, I need to implement constructor from
    //     initializer list.
    std::vector<BufferElement> m_elements;
    uint32_t m_stride;
  };

  enum class VertexBufferUsage
  {
    None = 0, Static = 1, Dynamic = 2
  };

  //------------------------------------------------------------------------------------------------
  // VertexBuffer
  //------------------------------------------------------------------------------------------------

  class VertexBuffer : public Resource
  {
  private:

    void Init(void* data, uint32_t size, VertexBufferUsage a_usage);
    void Init(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);

  public:

    VertexBuffer();
    ~VertexBuffer();

    static Ref<VertexBuffer> Create(void* a_data, 
                                    uint32_t a_size, 
                                    VertexBufferUsage a_usage = VertexBufferUsage::Static);

    static Ref<VertexBuffer> Create(uint32_t a_size, 
                                    VertexBufferUsage a_usage = VertexBufferUsage::Static);

    void SetData(void* data, uint32_t size, uint32_t offset = 0);
    void Bind() const;

    BufferLayout const& GetLayout() const;
    void SetLayout(BufferLayout const& layout);

    uint32_t GetSize() const;

  private:

    uint32_t m_size;
    VertexBufferUsage m_usage;
    BufferLayout m_layout;
  };

  //------------------------------------------------------------------------------------------------
  // IndexBuffer
  //------------------------------------------------------------------------------------------------

  class IndexBuffer : public Resource
  {
  private:

    void Init(void* data, uint32_t size);

  public:
    typedef uint16_t intType;
  public:

    static Ref<IndexBuffer> Create(void* a_data, uint32_t a_size);

    IndexBuffer();
     ~IndexBuffer();

    void SetData(void* data, uint32_t size, uint32_t offset);
    void Bind() const;

    uint32_t GetCount() const;
    uint32_t GetSize() const;

  private:

    uint32_t m_size; //buffer size
  };

}
#endif