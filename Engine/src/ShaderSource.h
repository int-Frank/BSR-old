//@group Renderer

#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

#include <string>
#include "ShaderUniform.h"

namespace Engine
{
  enum class StrType
  {
    Source,
    Path
  };

  struct ShaderSourceElement
  {
    ShaderDomain  domain;
    StrType       strType;
    std::string   str;

    ShaderSourceElement(ShaderDomain, StrType, std::string const&);
    ShaderSourceElement();
  };

  class ShaderSource
  {
  public:

    ShaderSource();
    ShaderSource(std::initializer_list<ShaderSourceElement> const&);
    std::string const& Get(ShaderDomain) const;

    void Clear();

  private:

    std::string m_src[SD32(COUNT)];
  };

}

#endif