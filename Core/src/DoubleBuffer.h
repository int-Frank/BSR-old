#ifndef DOUBLEBUFFER_H
#define DOUBLEBUFFER_H

#include <mutex>
#include <atomic>
#include <condition_variable>

#include "core_utils.h"
#include "PODArray.h"
#include "Membuffer.h"
#include "core_Assert.h"

//many producers, one consumer
template<size_t SIZE,
         size_t ALIGNMENT = __STDCPP_DEFAULT_NEW_ALIGNMENT__>
class DoubleBuffer
{
  typedef Core::MemBuffer<SIZE, ALIGNMENT> MemBuffer;
public:

  class Ref
  {
    friend class DoubleBuffer;
    Ref(std::atomic<size_t> *, void *);
  public:

    Ref();
    ~Ref();

    Ref(Ref const&);
    Ref& operator=(Ref const&);

    Ref(Ref &&);
    Ref& operator=(Ref &&);

    void * GetBuffer() const;

  private:
    std::atomic<size_t> * m_nProdRefs;
    void * m_buf;
  };

public:

  DoubleBuffer();
  ~DoubleBuffer();

  DoubleBuffer(DoubleBuffer const &) = delete;
  DoubleBuffer& operator=(DoubleBuffer const &) = delete;

  //Can be called from any thread.
  //Allocates on the current producer buffer
  Ref Allocate(size_t);

  void Swap();

  //Get the current consumer buffer
  MemBuffer& GetBuffer() const;

  //Get the number of Refs which currently have access to the producer buffer. 
  size_t GetCurrentRefCount();

private:

  int                     m_producerIndex;
  MemBuffer               m_buffer[2];
  std::atomic<size_t>     m_nProdRefs[2];
  std::mutex              m_mutex;
  std::condition_variable m_cv;
};

template<size_t SIZE, size_t ALIGNMENT>
DoubleBuffer<SIZE, ALIGNMENT>::Ref::Ref()
  : m_nProdRefs(nullptr)
  , m_buf(nullptr)
{

}

template<size_t SIZE, size_t ALIGNMENT>
DoubleBuffer<SIZE, ALIGNMENT>::Ref::Ref(std::atomic<size_t>* a_nProdRefs, void* a_buf)
  : m_nProdRefs(a_nProdRefs)
  , m_buf(a_buf)
{
  BSR_ASSERT(a_nProdRefs && a_buf, "");
  (*m_nProdRefs)++;
}

template<size_t SIZE, size_t ALIGNMENT>
DoubleBuffer<SIZE, ALIGNMENT>::Ref::~Ref()
{
  if (m_nProdRefs != nullptr)
    (*m_nProdRefs)--;
}

template<size_t SIZE, size_t ALIGNMENT>
DoubleBuffer<SIZE, ALIGNMENT>::Ref::Ref(Ref const& a_other)
  : m_nProdRefs(a_other.m_nProdRefs)
  , m_buf(a_other.m_buf)
{
  if (m_nProdRefs != nullptr)
    (*m_nProdRefs)++;
}

template<size_t SIZE, size_t ALIGNMENT>
typename DoubleBuffer<SIZE, ALIGNMENT>::Ref & 
DoubleBuffer<SIZE, ALIGNMENT>::Ref::operator=(Ref const& a_other)
{
  if (m_nProdRefs != nullptr)
    BSR_ASSERT(a_other.m_nProdRefs == m_nProdRefs, "These two Refs come from different DoubleBuffers!");

  if (this != &a_other)
  {
    m_buf = a_other.m_buf;
    if (m_nProdRefs == nullptr)
      (*a_other.m_nProdRefs)++;
    m_nProdRefs = a_other.m_nProdRefs;
  }

  return *this;
}

template<size_t SIZE, size_t ALIGNMENT>
DoubleBuffer<SIZE, ALIGNMENT>::Ref::Ref(Ref && a_other)
  : m_nProdRefs(a_other.m_nProdRefs)
  , m_buf(a_other.m_buf)
{
  a_other.m_buf = nullptr;
  a_other.m_nProdRefs = nullptr;
}

template<size_t SIZE, size_t ALIGNMENT>
typename DoubleBuffer<SIZE, ALIGNMENT>::Ref&
DoubleBuffer<SIZE, ALIGNMENT>::Ref::operator=(Ref && a_other)
{
  if (m_nProdRefs != nullptr)
    BSR_ASSERT(a_other.m_nProdRefs == m_nProdRefs, "These two Refs come from different DoubleBuffers!");

  if (this != &a_other)
  {
    m_buf = a_other.m_buf;
    m_nProdRefs = a_other.m_nProdRefs;
    a_other.m_buf = nullptr;
    a_other.m_nProdRefs = nullptr;
  }

  return *this;
}

template<size_t SIZE, size_t ALIGNMENT>
void * DoubleBuffer<SIZE, ALIGNMENT>::Ref::GetBuffer() const
{
  return m_buf;
}

template<size_t SIZE, size_t ALIGNMENT>
DoubleBuffer<SIZE, ALIGNMENT>::DoubleBuffer()
  : m_producerIndex(0)
  , m_nProdRefs{0, 0}
{

}

template<size_t SIZE, size_t ALIGNMENT>
DoubleBuffer<SIZE, ALIGNMENT>::~DoubleBuffer()
{
  //Producer threads should have been terminated before buffer is destroyed.
  BSR_ASSERT(m_nProdRefs[0] == 0 && m_nProdRefs[1] == 0, "")
}

template<size_t SIZE, size_t ALIGNMENT>
typename DoubleBuffer<SIZE, ALIGNMENT>::Ref
DoubleBuffer<SIZE, ALIGNMENT>::Allocate(size_t a_size)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return Ref(&m_nProdRefs[m_producerIndex], m_buffer[m_producerIndex].Allocate(a_size));
}

template<size_t SIZE, size_t ALIGNMENT>
void DoubleBuffer<SIZE, ALIGNMENT>::Swap()
{
  //Save the consumer index
  int ind = m_producerIndex;

  //Set new threads to allocate on the new buffer
  m_mutex.lock();
  m_producerIndex = (m_producerIndex + 1) % 2;
  m_nProdRefs[m_producerIndex] = 0;
  m_mutex.unlock();

  //Wait until no thread is accessing the now consumer buffer.
  if (m_nProdRefs[ind] != 0)
  {
    std::mutex mut;
    std::unique_lock<std::mutex> lock(mut);
    m_cv.wait(lock, [this, ind = ind]
      {
        return m_nProdRefs[ind] == 0;
      });
  }
}

template<size_t SIZE, size_t ALIGNMENT>
typename DoubleBuffer<SIZE, ALIGNMENT>::MemBuffer& 
DoubleBuffer<SIZE, ALIGNMENT>::GetBuffer() const
{
  ind = (m_producerIndex + 1) % 2;
  return m_buffer[ind];
}

template<size_t SIZE, size_t ALIGNMENT>
size_t DoubleBuffer<SIZE, ALIGNMENT>::GetCurrentRefCount()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_nProdRefs[m_producerIndex];
}

#endif