//@group Memory

#include "Memory.h"

namespace Engine
{
  namespace impl
  {
    namespace TRef
    {
      std::mutex mutex;
      Core::MemBuffer<10 * 1024 * 1024, __STDCPP_DEFAULT_NEW_ALIGNMENT__> buf;
    }
  }
}