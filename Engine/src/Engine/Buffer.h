#ifndef EN_BUFFER_H
#define EN_BUFFER_H

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

#include "core_Assert.h"

namespace Engine 
{
  typedef uint32_t RendererID;

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
    std::vector<BufferElement> m_elements;
    uint32_t m_stride;
  };

  enum class VertexBufferUsage
  {
    None = 0, Static = 1, Dynamic = 2
  };

  class IVertexBuffer
  {
  public:
    virtual ~IVertexBuffer() {}

    virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;
    virtual void Bind() const = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    virtual unsigned int GetSize() const = 0;
    virtual RendererID GetRendererID() const = 0;

    static std::shared_ptr<IVertexBuffer> Create(void* data, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Static);
    static std::shared_ptr<IVertexBuffer> Create(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);

  };

  class IIndexBuffer
  {
  public:
    virtual ~IIndexBuffer() {}

    virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;
    virtual void Bind() const = 0;

    virtual uint32_t GetCount() const = 0;

    virtual unsigned int GetSize() const = 0;
    virtual RendererID GetRendererID() const = 0;

    static std::shared_ptr<IIndexBuffer> Create(void* data, uint32_t size = 0);
  };

}
#endif