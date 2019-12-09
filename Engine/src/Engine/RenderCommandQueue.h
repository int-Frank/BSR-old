#ifndef RENDERCOMMANDQUEUE_H
#define RENDERCOMMANDQUEUE_H

#include "Memory.h"
#include <stdint.h>

#include "DgDynamicArray.h"
#include "PODArray.h"
#include "RenderState.h"

namespace Engine
{
  class RenderSortCriterion
  {

  };

  typedef void(*RenderCommandFn)(void*);

  class RenderCommandQueue
  {
  public:

    RenderCommandQueue();
    ~RenderCommandQueue();

    void* Allocate(RenderState, RenderCommandFn, uint32_t size);
    void Execute();

    void PushCriterion(Ref<RenderSortCriterion>);
    void ClearCriterion();

    void Sort();

  private:


    struct KV
    {
      RenderState renderState;
      uint32_t    offset;
    };

    struct SubArray
    {
      uint32_t offset;
      uint32_t count;
    };

    Dg::DynamicArray<Ref<RenderSortCriterion>>  m_sortCriterion;
    PODArray<SubArray>              m_sortableSegs;
    PODArray<KV>                    m_renderQueue;

    //Function to execute the lambda (64)| the lambda we added
    uint32_t                        m_currentIndex;
    PODArray<uint8_t>               m_commandBuffer;

  };
}

#endif