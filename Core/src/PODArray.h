#ifndef PODARRAY_H
#define PODARRAY_H

#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdint.h>

#include "impl/DgPoolSizeManager.h"

template<typename T, typename = std::enable_if_t<std::is_pod<T>::value>>
class PODArray
{
public:

  PODArray()
    : m_pData(nullptr)
    , m_nItems(0)
    , m_poolSize(1024)
  {
    resize(1024);
  }

  PODArray(size_t a_size)
    : m_pData(nullptr)
    , m_nItems(0)
    , m_poolSize(1024)
  {
    resize(a_size);
  }

  void resize(size_t a_size)
  {
    T * temp = static_cast<T*>(realloc(m_pData, m_poolSize.PeekNextPoolSize() * sizeof(T)));
    if (temp == nullptr)
      throw std::exception("Failed to allocate memory");
    m_pData = temp;
    m_poolSize.SetNextPoolSize();
  }

  void push_back(T const & a_item)
  {
    if (m_nItems >= m_poolSize.GetSize())
      extend();

    m_pData[m_nItems] = a_item;
    m_nItems++;
  }

  void push_back(T * a_buf, size_t a_count)
  {
    size_t newSize = m_nItems + a_count;
    if (newSize >= m_poolSize.GetSize())
      resize(newSize + 1);
    memcpy(&m_pData[m_nItems], a_buf, a_count);
    m_nItems += a_count;
  }

  void pop_back()
  {
    m_nItems--;
  }

  size_t size() const
  {
    return m_nItems;
  }

  void clear()
  {
    m_nItems = 0;
  }

  void erase_swap(size_t a_ind)
  {
    m_pData[a_ind] = m_pData[m_nItems - 1];
    m_nItems--;
  }

  T & operator[](size_t a_ind)
  {
    return m_pData[a_ind];
  }

  T const & operator[](size_t) const
  {
    return m_pData[a_ind];
  }

private:

  void extend()
  {
    resize(m_poolSize.PeekNextPoolSize());
  }

private:

  T*              m_pData;
  size_t          m_nItems;
  Dg::PoolSizeManager m_poolSize;
};

#endif