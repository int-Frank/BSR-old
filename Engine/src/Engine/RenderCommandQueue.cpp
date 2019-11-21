#include "RenderCommandQueue.h"

namespace Engine
{
  RenderCommandQueue::RenderCommandQueue()
    : m_commandBuffer(1 * 1024 * 1024)
    , m_currentIndex(0)
  {

  }

  RenderCommandQueue::~RenderCommandQueue()
  {

  }

  void* RenderCommandQueue::Allocate(RenderState a_state, 
                                     RenderCommandFn a_fn, 
                                     uint32_t a_size)
  {
    int align = 8;

    //Move to next aligned block
    m_currentIndex = m_currentIndex + ((align - (m_currentIndex % align)) % align);
    uint32_t requiredSize = m_currentIndex + a_size + 1;

    if (m_commandBuffer.mem_block_size() < requiredSize)
      m_commandBuffer.resize(requiredSize);

    KV kv;
    kv.renderState = a_state;
    kv.offset = m_currentIndex;
    m_renderQueue.push_back(kv);

    *(RenderCommandFn*)&m_commandBuffer[m_currentIndex] = a_fn;
    m_currentIndex += uint32_t(sizeof(RenderCommandFn));

    void * memory = &m_commandBuffer[m_currentIndex];
    m_currentIndex += a_size;

    return memory;
  }

  void RenderCommandQueue::Execute()
  {
    for (size_t i = 0; i < m_renderQueue.size(); i++)
    {
      uint32_t offset = m_renderQueue[i].offset;

      RenderCommandFn function = *(RenderCommandFn*)&m_commandBuffer[offset];
      void * data = &m_commandBuffer[offset + sizeof(RenderCommandFn)];
      function(data);
    }

    m_commandBuffer.clear();
    m_renderQueue.clear();
    m_sortableSegs.clear();
  }

  void RenderCommandQueue::PushCriterion(std::shared_ptr<RenderSortCriterion> a_crit)
  {
    m_sortCriterion.push_back(a_crit);
  }

  void RenderCommandQueue::ClearCriterion()
  {
    m_sortCriterion.clear();
  }

  void RenderCommandQueue::Sort()
  {

  }
}