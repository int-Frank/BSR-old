#include <iostream>
#include "GameMapConverter.h"

#include "Types.h"
#include "DgRNG.h"
#include "DgR3Vector.h"

int main()
{
  for (int i = 0; i < 20; i++)
  {
    vec3 p, v, n;
    p[0] = Dg::RNG::GetUniform<float>(-10.0f, 10.0f);
    p[1] = Dg::RNG::GetUniform<float>(-10.0f, 10.0f);
    p[2] = 0.0f;

    v[0] = Dg::RNG::GetUniform<float>(-10.0f, 10.0f);
    v[1] = Dg::RNG::GetUniform<float>(-10.0f, 10.0f);
    v[2] = 0.0f;

    n[0] = Dg::RNG::GetUniform<float>(-10.0f, 10.0f);
    n[1] = Dg::RNG::GetUniform<float>(-10.0f, 10.0f);
    n[2] = 0.0f;

    float v0 = (p + v).Dot(n);
    float v1 = p.Dot(n) + v.Dot(n);
    char f = 0;

  }

  char brk;
  std::cin >> brk;
  return 0;


  GameMapConverter converter;

  try
  {
    converter.Convert();
  }
  catch(const std::runtime_error& re)
  {
    // speciffic handling for runtime_error
    std::cout << "Runtime error: " << re.what() << std::endl;
  }
  catch(const std::exception& ex)
  {
    // speciffic handling for all exceptions extending std::exception, except
    // std::runtime_error which is handled explicitly
    std::cout << "Error occurred: " << ex.what() << std::endl;
  }
  catch(...)
  {
    // catch any other errors (that we have no information about)
    std::cout << "Unknown failure occurred. Possible memory corruption" << std::endl;
  }

  return 0;
}