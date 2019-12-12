#ifndef MEMBUFFER_H
#define MEMBUFFER_H

#include <exception>
#include <cstdlib>
#include "DgMath.h"
#include "core_Assert.h"
#include "PODArray.h"
#include "core_utils.h"

namespace Core
{
  template<size_t ALIGNMENT = __STDCPP_DEFAULT_NEW_ALIGNMENT__>
  class MemBufferDynamic
  {
    static size_t const s_defaultSize = 1024;

  public:

    typedef std::ptrdiff_t Index;

  public:

    MemBufferDynamic();
    ~MemBufferDynamic();

    MemBufferDynamic(MemBufferDynamic const& a_other);
    MemBufferDynamic& operator=(MemBufferDynamic const& a_other);

    Index Allocate(size_t);
    void * GetAddress(Index);
    void clear();
    size_t size() const;

  private:

    size_t          m_cursor;
    PODArray<byte>  m_memblock;
  };

  template<size_t SIZE,
           size_t ALIGNMENT = __STDCPP_DEFAULT_NEW_ALIGNMENT__>
  class MemBuffer
  {
    MemBuffer(MemBuffer const&) = delete;
    MemBuffer& operator=(MemBuffer const&) = delete;

  public:

    MemBuffer();
    ~MemBuffer();

    void* Allocate(size_t a_size);
    void clear();
    size_t size() const;

  private:

    size_t  m_cursor;
    byte *  m_memblock;
  };

  //--------------------------------------------------------------------------------------------
  // MemBufferDynamic
  //--------------------------------------------------------------------------------------------
  template<size_t ALIGNMENT>
  MemBufferDynamic<ALIGNMENT>::MemBufferDynamic()
    : m_cursor(0)
  {

  }

  template<size_t ALIGNMENT>
  MemBufferDynamic<ALIGNMENT>::~MemBufferDynamic()
  {

  }

  template<size_t ALIGNMENT>
  MemBufferDynamic<ALIGNMENT>::MemBufferDynamic(MemBufferDynamic const& a_other)
    : m_cursor(a_other.m_cursor)
    , m_memblock(a_other.m_memblock)
  {

  }

  template<size_t ALIGNMENT>
  MemBufferDynamic<ALIGNMENT>& MemBufferDynamic<ALIGNMENT>::operator=(MemBufferDynamic const& a_other)
  {
    if (this != &a_other)
    {
      m_cursor = a_other.m_cursor;
      m_memblock = a_other.m_memblock;
    }
    return *this;
  }

  template<size_t ALIGNMENT>
  typename MemBufferDynamic<ALIGNMENT>::Index
    MemBufferDynamic<ALIGNMENT>::Allocate(size_t a_size)
  {
    size_t newCursor = Dg::ToNextAlignment<size_t, ALIGNMENT>(m_cursor + a_size);
    if (newCursor > m_memblock.size())
      m_memblock.resize(newCursor);
    size_t oldCursor = m_cursor;
    m_cursor = newCursor;
    return oldCursor;
  }

  template<size_t ALIGNMENT>
  void * MemBufferDynamic<ALIGNMENT>::GetAddress(Index a_index)
  {
    return m_memblock.data() + a_index;
  }

  template<size_t ALIGNMENT>
  void MemBufferDynamic<ALIGNMENT>::clear()
  {
    m_cursor = 0;
  }

  template<size_t ALIGNMENT>
  size_t MemBufferDynamic<ALIGNMENT>::size() const
  {
    return m_cursor;
  }
  //--------------------------------------------------------------------------------------------
  // MemBuffer
  //--------------------------------------------------------------------------------------------
  template<size_t SIZE, size_t ALIGNMENT>
  MemBuffer<SIZE, ALIGNMENT>::MemBuffer()
    : m_memblock(nullptr)
    , m_cursor(0)
  {
    m_memblock = (unsigned char*)malloc(SIZE);
    if (m_memblock == nullptr)
      throw std::exception("MemBuffer failed to allocate!");
  }

  template<size_t SIZE, size_t ALIGNMENT>
  MemBuffer<SIZE, ALIGNMENT>::~MemBuffer()
  {
    free(m_memblock);
  }

  template<size_t SIZE, size_t ALIGNMENT>
  void* MemBuffer<SIZE, ALIGNMENT>::Allocate(size_t a_size)
  {
    BSR_ASSERT(m_cursor + a_size < SIZE, "MemBuffer out of memory!");
    void* mem = &m_memblock[m_cursor];
    m_cursor += a_size;
    m_cursor = Dg::ToNextAlignment<size_t, ALIGNMENT>(m_cursor);
    return mem;
  }

  template<size_t SIZE, size_t ALIGNMENT>
  void MemBuffer<SIZE, ALIGNMENT>::clear()
  {
    m_cursor = 0;
  }

  template<size_t SIZE, size_t ALIGNMENT>
  size_t MemBuffer<SIZE, ALIGNMENT>::size() const
  {
    return m_cursor;
  }
}

#endif