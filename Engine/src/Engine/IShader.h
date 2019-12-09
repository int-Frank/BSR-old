#ifndef EN_ISHADER_H
#define EN_ISHADER_H

#include <string>
#include "core_ErrorCodes.h"

namespace Engine
{
  class IShader
  {
  public:

    virtual ~IShader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Ref<IShader> Create(std::string const & vs, std::string const & fs);

  };
}

#endif