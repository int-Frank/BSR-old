//@group Renderer

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
    static size_t const s_cmdBufSize = 1 * 1024 * 1024;
    static size_t const s_outBufSize = 1 * 1024 * 1024;
    static size_t const s_memBufSize = 10 * 1024 * 1024;

  public:

    RenderCommandQueue();
    ~RenderCommandQueue();

    //Main thread...
    void * AllocateForCommand(RenderState, RenderCommandFn, uint32_t size);
    PODArray<void*> GetOutputCommands();

    //Render thread
    void * AllocateForOutput(RenderCommandFn, uint32_t size);
    void Swap();
    void Execute();

    void * Allocate(uint32_t size);

    void PushCriterion(Ref<RenderSortCriterion>);
    void ClearCriterion();

    void Sort();

  private:

    struct SubArray
    {
      uint32_t index;
      uint32_t count;
    };

    struct Buffer
    {
      Buffer(size_t memBufSize);
      void Clear();

      MemBuffer       buf;
      PODArray<void*> allocs;
    };

    PODArray<uint32_t>  m_sortedCommands;
    int                 m_writeIndex;
    Buffer              m_outputBuffer[2];
    Buffer              m_commandBuffer[2];
    MemBuffer           m_mem[2];

    Dg::DynamicArray<Ref<RenderSortCriterion>>  m_sortCriterion;
    PODArray<SubArray>              m_sortableSegs;

  };
}

#endif