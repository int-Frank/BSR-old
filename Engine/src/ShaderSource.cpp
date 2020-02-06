//@group Renderer

#include <fstream>
#include <sstream>

#include "ShaderSource.h"
#include "Serialize.h"

namespace Engine
{
  //------------------------------------------------------------------------------------------------
  // Helpful functions
  //------------------------------------------------------------------------------------------------
  static std::string RemoveComments(std::string const& a_src)
  {
    std::stringstream ss(a_src);
    std::string result;
    char c;
    while (ss.get(c))
    {
      if (c == '/')
      {
        if (ss.get(c))
        {
          if (c == '/')
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          else if (c == '*')
          {
            while (ss.get(c))
            {
              if (c == '*')
              {
                if (ss.get(c))
                {
                  if (c == '/')
                    break;
                }
              }
            }
          }
          else
          {
            result += '/';
            result += c;
          }
        }
        else
        {
          result += c;
          break;
        }
      }
      else
        result += c;
    }
    return result;
  }

  //------------------------------------------------------------------------------------------------
  // ShaderSourceElement
  //------------------------------------------------------------------------------------------------
  ShaderSourceElement::ShaderSourceElement(ShaderDomain a_domain, StrType a_type, std::string const& a_str)
    : domain(a_domain)
    , strType(a_type)
    , str(a_str)
  {

  }

  ShaderSourceElement::ShaderSourceElement()
    : domain(ShaderDomain::INVALID)
    , strType(StrType::Source)
  {

  }

  //------------------------------------------------------------------------------------------------
  // ShaderSource
  //------------------------------------------------------------------------------------------------

  ShaderSource::ShaderSource()
  {
  
  }

  ShaderSource::ShaderSource(std::initializer_list<ShaderSourceElement> const & a_list)
  {
    Clear();

    for (auto const & ele : a_list)
    {
      if (ele.domain == ShaderDomain::INVALID)
      {
        LOG_WARN("'INVALID' passed to ShaderSource::ShaderSource()");
        continue;
      }

      if (ele.domain == ShaderDomain::COUNT)
      {
        LOG_WARN("'COUNT' passed to ShaderSource::ShaderSource()");
        continue;
      }

      if (ele.strType == StrType::Source)
        m_src[static_cast<uint32_t>(ele.domain)] = RemoveComments(ele.str);
      else
      {
        std::ifstream ifs(ele.str);
        if (!ifs.good())
        {
          LOG_WARN("ShaderSource::ShaderSource() failed to open file '{}'", ele.str.c_str());
          continue;
        }

        std::string content((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));
        m_src[static_cast<uint32_t>(ele.domain)] = RemoveComments(content);
      }
    }
  }

  std::string const& ShaderSource::Get(ShaderDomain a_domain) const
  {
    if (a_domain == ShaderDomain::INVALID)
    {
      LOG_WARN("'INVALID' passed to ShaderSource::Get()");
      return std::string();
    }

    if (a_domain == ShaderDomain::COUNT)
    {
      LOG_WARN("'COUNT' passed to ShaderSource::Get()");
      return std::string();
    }

    return m_src[static_cast<uint32_t>(a_domain)];
  }

  void ShaderSource::Clear()
  {
    for (uint32_t i = 0; i < SD32(COUNT); i++)
      m_src[i].clear();
  }
}