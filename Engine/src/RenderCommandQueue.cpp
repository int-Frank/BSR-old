//@group Renderer

#include "RenderCommandQueue.h"
#include "core_Log.h"

namespace Engine
{
  RenderCommandQueue::Buffer::Buffer(size_t a_size)
    : buf(a_size)
  {

  }

  void RenderCommandQueue::Buffer::Clear()
  {
    buf.clear();
    allocs.clear();
  }

  RenderCommandQueue::RenderCommandQueue()
    : m_commandBuffer{Buffer(s_cmdBufSize), Buffer(s_cmdBufSize)}
    , m_mem{MemBuffer(s_memBufSize), MemBuffer(s_memBufSize)}
    , m_writeIndex(0)
  {

  }

  RenderCommandQueue::~RenderCommandQueue()
  {

  }

  void * RenderCommandQueue::Allocate(uint32_t a_size)
  {
    return m_mem[m_writeIndex].Allocate(a_size);
  }

  //Main thread (producer)...
  void* RenderCommandQueue::AllocateForCommand(RenderState a_state, 
                                               RenderCommandFn a_fn, 
                                               uint32_t a_size)
  {
    void* ptr = m_commandBuffer[m_writeIndex].buf.Allocate(sizeof(RenderState) + sizeof(RenderCommandFn) + a_size);
    m_commandBuffer[m_writeIndex].allocs.push_back(ptr);

    *static_cast<RenderState*>(ptr) = a_state;
    ptr = static_cast<void*>(static_cast<byte*>(ptr) + sizeof(RenderState));

    *static_cast<RenderCommandFn*>(ptr) = a_fn;
    ptr = static_cast<void*>(static_cast<byte*>(ptr) + sizeof(RenderCommandFn));

    return ptr;
  }

  //Render thread (consumer)...
  //The render thread could just repeat this...
  void RenderCommandQueue::Execute()
  {
    Sort();

    int readInd = (m_writeIndex + 1) % 2;
    for (size_t i = 0; i < m_sortedCommands.size(); i++)
    {
      uint32_t ind = m_sortedCommands[i];

      void * ptr = m_commandBuffer[readInd].allocs[ind];
      ptr = static_cast<void*>(static_cast<byte*>(ptr) + sizeof(RenderState));

      RenderCommandFn function = *(RenderCommandFn*)ptr;
      ptr = static_cast<void*>(static_cast<byte*>(ptr) + sizeof(RenderCommandFn));

      function(ptr);
    }
  }

  void RenderCommandQueue::Swap()
  {
    int readInd = (m_writeIndex + 1) % 2;
    m_commandBuffer[readInd].Clear();
    m_mem[readInd].clear();
    m_writeIndex = readInd;
  }

  void RenderCommandQueue::PushCriterion(Ref<RenderSortCriterion> a_crit)
  {
    m_sortCriterion.push_back(a_crit);
  }

  void RenderCommandQueue::ClearCriterion()
  {
    m_sortCriterion.clear();
  }

  void RenderCommandQueue::Sort()
  {
    //No sort for now...
    m_sortedCommands.clear();
    int readInd = (m_writeIndex + 1) % 2;
    for (uint32_t i = 0; i < (uint32_t)m_commandBuffer[readInd].allocs.size(); i++)
    {
      m_sortedCommands.push_back(i);
    }

    m_sortableSegs.clear();
  }
}