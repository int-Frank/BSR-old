
#include <cstring>
#include <fstream>

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

  std::string ImportTextFile(std::string const& a_filepath)
  {
    std::string content;
    std::ifstream ifs(a_filepath);
    if (ifs)
    {
      content.assign((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));
    }
    return content;
  }
}