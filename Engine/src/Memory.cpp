//@group Memory

#include "Memory.h"

namespace Engine
{
  namespace impl
  {
    namespace TRef
    {
      std::mutex mutex;
      MemBuffer buf(10 * 1024 * 1024);
    }
  }
}