//@group Renderer

#include "ShaderUtils.h"
#include "core_Assert.h"

namespace Engine
{

  static char const* ShaderDataType_strings[] =
  {
    "Invalid Type",
    "bool", "int", "uint", "float", "double",
    "bvec2", "bvec3", "bvec4",
    "ivec2", "ivec3", "ivec4",
    "uvec2", "uvec3", "uvec4",
    "vec2", "vec3", "vec4",
    "dvec2", "dvec3", "dvec4",
    "mat2", "mat3", "mat4",
    "mat2x3", "mat2x4",
    "mat3x2", "mat3x4",
    "mat4x2", "mat4x3",
    "dmat2", "dmat3", "dmat4",
    "dmat2x3", "dmat2x4",
    "dmat3x2", "dmat3x4",
    "dmat4x2", "dmat4x3",
  };

  static char const * ShaderResourceType_strings[] = 
  {
    "Invalid Type",
    "sampler2D", "samplerCube"
  };

  uint32_t SizeOfShaderDataType(ShaderDataType const a_type)
  {
    switch (a_type)
    {
      case ShaderDataType::BOOL:
      case ShaderDataType::INT:
      case ShaderDataType::UINT:
      case ShaderDataType::FLOAT:
        return 4;
      case ShaderDataType::DOUBLE:
      case ShaderDataType::VEC2:
      case ShaderDataType::BVEC2:
      case ShaderDataType::IVEC2:
      case ShaderDataType::UVEC2:
        return 8;
      case ShaderDataType::VEC3:
      case ShaderDataType::BVEC3:
      case ShaderDataType::IVEC3:
      case ShaderDataType::UVEC3:
        return 12;
      case ShaderDataType::DVEC2:
      case ShaderDataType::BVEC4:
      case ShaderDataType::IVEC4:
      case ShaderDataType::UVEC4:
      case ShaderDataType::MAT2:
        return 16;
      case ShaderDataType::DVEC3:
      case ShaderDataType::MAT2x3:
      case ShaderDataType::MAT3x2:
        return 24;
      case ShaderDataType::DVEC4:
      case ShaderDataType::MAT2x4:
      case ShaderDataType::MAT4x2:
      case ShaderDataType::DMAT2:
        return 32;
      case ShaderDataType::MAT3:
        return 36;
      case ShaderDataType::MAT3x4:
      case ShaderDataType::MAT4x3:
      case ShaderDataType::DMAT2x3:
      case ShaderDataType::DMAT3x2:
        return 48;
      case ShaderDataType::MAT4:
      case ShaderDataType::DMAT2x4:
      case ShaderDataType::DMAT4x2:
        return 64;
      case ShaderDataType::DMAT3:
        return 72;
      case ShaderDataType::DMAT3x4:
      case ShaderDataType::DMAT4x3:
        return 96;
      case ShaderDataType::DMAT4:
        return 128;
    }
    BSR_ASSERT(false, "Unrecognised type!");
    return 0;
  }

  ShaderDataType StringToType(std::string const & a_type)
  {
    for (uint32_t i = 1; i < static_cast<uint32_t>(ShaderDataType::STRUCT); i++)
    {
      if (a_type == std::string(ShaderDataType_strings[i]))
        return static_cast<ShaderDataType>(i);
    }
    BSR_ASSERT(false, "Unrecognised type!");
    return ShaderDataType::NONE;
  }

  std::string TypeToString(ShaderDataType a_type)
  {
    return ShaderDataType_strings[static_cast<uint32_t>(a_type)];
  }

  ShaderResourceType StringToShaderResourceType(std::string const & a_type)
  {
    for (uint32_t i = 1; i <= static_cast<uint32_t>(ShaderResourceType::TEXTURECUBE); i++)
    {
      if (a_type == std::string(ShaderResourceType_strings[i]))
        return static_cast<ShaderResourceType>(i);
    }
    BSR_ASSERT(false, "Unrecognised type!");
    return ShaderResourceType::NONE;
  }

  std::string ShaderResourceTypeToString(ShaderResourceType a_type)
  {
    return ShaderResourceType_strings[static_cast<uint32_t>(a_type)];
  }

  ShaderDataType ConvertToShaderDataType(uint32_t a_val)
  {
    if (a_val < static_cast<uint32_t>(ShaderDataType::STRUCT))
      return static_cast<ShaderDataType>(a_val);
    BSR_ASSERT(false, "Unrecognised type!");
    return ShaderDataType::NONE;
  }

  ShaderDataType uint_ToShaderDataType(uint32_t a_val)
  {
    if (a_val <= static_cast<uint32_t>(ShaderDataType::STRUCT))
      return static_cast<ShaderDataType>(a_val);
    return ShaderDataType::NONE;
  }

  uint32_t GetComponentCount(ShaderDataType const a_type)
  {
    switch (a_type)
    {
      case ShaderDataType::BOOL:
      case ShaderDataType::INT:
      case ShaderDataType::UINT:
      case ShaderDataType::FLOAT:
      case ShaderDataType::DOUBLE:
        return 1;
      case ShaderDataType::BVEC2:
      case ShaderDataType::IVEC2:
      case ShaderDataType::UVEC2:
      case ShaderDataType::VEC2:
      case ShaderDataType::DVEC2:
        return 2;
      case ShaderDataType::BVEC3:
      case ShaderDataType::IVEC3:
      case ShaderDataType::UVEC3:
      case ShaderDataType::VEC3:
      case ShaderDataType::DVEC3:
        return 3;
      case ShaderDataType::BVEC4:
      case ShaderDataType::IVEC4:
      case ShaderDataType::UVEC4:
      case ShaderDataType::VEC4:
      case ShaderDataType::DVEC4:
      case ShaderDataType::MAT2:
      case ShaderDataType::DMAT2:
        return 4;
      case ShaderDataType::MAT2x3:
      case ShaderDataType::MAT3x2:
      case ShaderDataType::DMAT2x3:
      case ShaderDataType::DMAT3x2:
        return 6;
      case ShaderDataType::MAT2x4:
      case ShaderDataType::MAT4x2:
      case ShaderDataType::DMAT2x4:
      case ShaderDataType::DMAT4x2:
        return 8;
      case ShaderDataType::MAT3:
      case ShaderDataType::DMAT3:
        return 9;
      case ShaderDataType::MAT3x4:
      case ShaderDataType::MAT4x3:
      case ShaderDataType::DMAT3x4:
      case ShaderDataType::DMAT4x3:
        return 12;
      case ShaderDataType::MAT4:
      case ShaderDataType::DMAT4:
        return 16;
    }
    BSR_ASSERT(false, "Unrecognised type!");
    return 0;
  }
}