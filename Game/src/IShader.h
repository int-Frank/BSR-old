#ifndef ISHADER_H
#define ISHADER_H

#include <string>
#include "ErrorCodes.h"

class IShader
{
public:

  virtual ~IShader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

};

#endif