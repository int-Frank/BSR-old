#ifndef IGRAPHICSCONTEXT_H
#define IGRAPHICSCONTEXT_H

#include "ErrorCodes.h"

namespace Engine
{
  class IGraphicsContext
  {
  public:

    virtual ~IGraphicsContext(){};

    virtual ErrorCode Init() = 0;
    virtual void SwapBuffers() = 0;

  private:

  };
}

#endif