#ifndef MEMBUFFER_H
#define MEMBUFFER_H

#include <exception>
#include <cstdlib>
#include "DgMath.h"

namespace Core
{
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

    size_t          m_cursor;
    unsigned char * m_memblock;
  };

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