
#include <cstring>

#include "core_utils.h"

namespace Core
{
  void* AdvancePtr(void* a_ptr, size_t a_increment)
  {
    return static_cast<void*>(static_cast<byte*>(a_ptr) + a_increment);
  }

  void const * AdvancePtr(void const * a_ptr, size_t a_increment)
  {
    return static_cast<void const *>(static_cast<byte const *>(a_ptr) + a_increment);
  }
}