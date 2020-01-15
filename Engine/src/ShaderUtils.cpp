//@group Renderer

#include <glad/glad.h>

#include "ShaderUtils.h"
#include "core_Assert.h"

namespace Engine
{
  struct ShaderDataTypeData
  {
    ShaderDataType      dataType;
    ShaderDataClass     dataClass;
    ShaderDataBaseType  baseType;
    std::string         str;
    uint32_t            componentCount;
    ShaderDataType      matColumnType;
    ShaderDataType      matRowType;
    uint32_t            std140BaseAlignmentSingle;
    uint32_t            std140BaseAlignmentArray;
    uint32_t            std140StrideSingle;
    uint32_t            std140StrideArray;
  };

#define S1 4
#define DS1 8
#define V2 8
#define V3 12
#define V4 16
#define DV2 16
#define DV3 24
#define DV4 32

#define CR(c, r) (((c) << 16) | (r))
#define COLUMN(i) (i >> 16)
#define ROW(i) (i & 0xFFFF)

#define SET_FLAGS(Class, BaseType) (static_cast<uint32_t>(Class) | (static_cast<uint32_t>(BaseType) << 3))
#define GET_CLASS_FROM_FLAG(i) (i & 7)
#define GET_BASE_TYPE_FROM_FLAG(i) (i >> 3)

  static ShaderDataTypeData const SDT_data[] = 
  {
   //      Type           |         Class            |      Base type             |      string      |cc | matrix column type  |    matrix row type      |ubs |uba |uss                  |usa      
    {ShaderDataType::NONE,    ShaderDataClass::None,    ShaderDataBaseType::None,     "Invalid Type", 0,  ShaderDataType::NONE,   ShaderDataType::NONE,   0,   0,   0,                    0},
    {ShaderDataType::BOOL,    ShaderDataClass::Scalar,  ShaderDataBaseType::Bool,     "bool",         1,  ShaderDataType::NONE,   ShaderDataType::NONE,   S1,  V4,  S1,                   V4},
    {ShaderDataType::INT,     ShaderDataClass::Scalar,  ShaderDataBaseType::Int,      "int",          1,  ShaderDataType::NONE,   ShaderDataType::NONE,   S1,  V4,  S1,                   V4},
    {ShaderDataType::UINT,    ShaderDataClass::Scalar,  ShaderDataBaseType::UInt,     "uint",         1,  ShaderDataType::NONE,   ShaderDataType::NONE,   S1,  V4,  S1,                   V4},
    {ShaderDataType::FLOAT,   ShaderDataClass::Scalar,  ShaderDataBaseType::Float,    "float",        1,  ShaderDataType::NONE,   ShaderDataType::NONE,   S1,  V4,  S1,                   V4},
    {ShaderDataType::DOUBLE,  ShaderDataClass::Scalar,  ShaderDataBaseType::Double,   "double",       1,  ShaderDataType::NONE,   ShaderDataType::NONE,   DS1, V4,  DS1,                  V4},
    {ShaderDataType::BVEC2,   ShaderDataClass::Vector,  ShaderDataBaseType::Bool,     "bvec2",        2,  ShaderDataType::NONE,   ShaderDataType::NONE,   V2,  V4,  V2,                   V4},
    {ShaderDataType::BVEC3,   ShaderDataClass::Vector,  ShaderDataBaseType::Bool,     "bvec3",        3,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V3,                   V4},
    {ShaderDataType::BVEC4,   ShaderDataClass::Vector,  ShaderDataBaseType::Bool,     "bvec4",        4,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V4,                   V4},
    {ShaderDataType::IVEC2,   ShaderDataClass::Vector,  ShaderDataBaseType::Int,      "ivec2",        2,  ShaderDataType::NONE,   ShaderDataType::NONE,   V2,  V4,  V2,                   V4},
    {ShaderDataType::IVEC3,   ShaderDataClass::Vector,  ShaderDataBaseType::Int,      "ivec3",        3,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V3,                   V4},
    {ShaderDataType::IVEC4,   ShaderDataClass::Vector,  ShaderDataBaseType::Int,      "ivec4",        4,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V4,                   V4},
    {ShaderDataType::UVEC2,   ShaderDataClass::Vector,  ShaderDataBaseType::UInt,     "uvec2",        2,  ShaderDataType::NONE,   ShaderDataType::NONE,   V2,  V4,  V2,                   V4},
    {ShaderDataType::UVEC3,   ShaderDataClass::Vector,  ShaderDataBaseType::UInt,     "uvec3",        3,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V3,                   V4},
    {ShaderDataType::UVEC4,   ShaderDataClass::Vector,  ShaderDataBaseType::UInt,     "uvec4",        4,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V4,                   V4},
    {ShaderDataType::VEC2,    ShaderDataClass::Vector,  ShaderDataBaseType::Float,    "vec2",         2,  ShaderDataType::NONE,   ShaderDataType::NONE,   V2,  V4,  V2,                   V4},
    {ShaderDataType::VEC3,    ShaderDataClass::Vector,  ShaderDataBaseType::Float,    "vec3",         3,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V3,                   V4},
    {ShaderDataType::VEC4,    ShaderDataClass::Vector,  ShaderDataBaseType::Float,    "vec4",         4,  ShaderDataType::NONE,   ShaderDataType::NONE,   V4,  V4,  V4,                   V4},
    {ShaderDataType::DVEC2,   ShaderDataClass::Vector,  ShaderDataBaseType::Double,   "dvec2",        2,  ShaderDataType::NONE,   ShaderDataType::NONE,   DV2, V4,  DV2,                  V4},
    {ShaderDataType::DVEC3,   ShaderDataClass::Vector,  ShaderDataBaseType::Double,   "dvec3",        3,  ShaderDataType::NONE,   ShaderDataType::NONE,   DV4, DV4, DV3,                  DV4},
    {ShaderDataType::DVEC4,   ShaderDataClass::Vector,  ShaderDataBaseType::Double,   "dvec4",        4,  ShaderDataType::NONE,   ShaderDataType::NONE,   DV4, DV4, DV4,                  DV4},
    {ShaderDataType::MAT2,    ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat2",         4,  ShaderDataType::VEC2,   ShaderDataType::VEC2,   V4,  V4,  CR(V4 * 2, V4 * 2),   CR(V4 * 2, V4 * 2)},
    {ShaderDataType::MAT2x2,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat2x2",       4,  ShaderDataType::VEC2,   ShaderDataType::VEC2,   V4,  V4,  CR(V4 * 2, V4 * 2),   CR(V4 * 2, V4 * 2)},
    {ShaderDataType::MAT3,    ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat3",         9,  ShaderDataType::VEC3,   ShaderDataType::VEC3,   V4,  V4,  CR(V4 * 3, V4 * 3),   CR(V4 * 3, V4 * 3)},
    {ShaderDataType::MAT3x3,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat3x3",       9,  ShaderDataType::VEC3,   ShaderDataType::VEC3,   V4,  V4,  CR(V4 * 3, V4 * 3),   CR(V4 * 3, V4 * 3)},
    {ShaderDataType::MAT4,    ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat4",         16, ShaderDataType::VEC4,   ShaderDataType::VEC4,   V4,  V4,  CR(V4 * 2, V4 * 2),   CR(V4 * 2, V4 * 2)},
    {ShaderDataType::MAT4x4,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat4x4",       16, ShaderDataType::VEC4,   ShaderDataType::VEC4,   V4,  V4,  CR(V4 * 4, V4 * 4),   CR(V4 * 4, V4 * 4)},
    {ShaderDataType::MAT2x3,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat2x3",       6,  ShaderDataType::VEC2,   ShaderDataType::VEC2,   V4,  V4,  CR(V4 * 4, V4 * 4),   CR(V4 * 4, V4 * 4)},
    {ShaderDataType::MAT2x4,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat2x4",       8,  ShaderDataType::VEC2,   ShaderDataType::VEC4,   V4,  V4,  CR(V4 * 2, V4 * 4),   CR(V4 * 2, V4 * 4)},
    {ShaderDataType::MAT3x2,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat3x2",       6,  ShaderDataType::VEC3,   ShaderDataType::VEC2,   V4,  V4,  CR(V4 * 3, V4 * 2),   CR(V4 * 3, V4 * 2)},
    {ShaderDataType::MAT3x4,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat3x4",       12, ShaderDataType::VEC3,   ShaderDataType::VEC4,   V4,  V4,  CR(V4 * 4, V4 * 4),   CR(V4 * 4, V4 * 4)},
    {ShaderDataType::MAT4x2,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat4x2",       8,  ShaderDataType::VEC4,   ShaderDataType::VEC2,   V4,  V4,  CR(V4 * 4, V4 * 2),   CR(V4 * 4, V4 * 2)},
    {ShaderDataType::MAT4x3,  ShaderDataClass::Matrix,  ShaderDataBaseType::Float,    "mat4x3",       12, ShaderDataType::VEC4,   ShaderDataType::VEC3,   V4,  V4,  CR(V4 * 4, V4 * 3),   CR(V4 * 4, V4 * 3)},
    {ShaderDataType::DMAT2,   ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat2",        4,  ShaderDataType::DVEC2,  ShaderDataType::DVEC2,  DV4, DV4, CR(DV4 * 2, DV4 * 2), CR(DV4 * 2, DV4 * 2)},
    {ShaderDataType::DMAT2x2, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat2x2",      4,  ShaderDataType::DVEC2,  ShaderDataType::DVEC2,  DV4, DV4, CR(DV4 * 2, DV4 * 2), CR(DV4 * 2, DV4 * 2)},
    {ShaderDataType::DMAT3,   ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat3",        9,  ShaderDataType::DVEC3,  ShaderDataType::DVEC3,  DV4, DV4, CR(DV4 * 3, DV4 * 3), CR(DV4 * 3, DV4 * 3)},
    {ShaderDataType::DMAT3x3, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat3x3",      9,  ShaderDataType::DVEC3,  ShaderDataType::DVEC3,  DV4, DV4, CR(DV4 * 3, DV4 * 3), CR(DV4 * 3, DV4 * 3)},
    {ShaderDataType::DMAT4,   ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat4",        16, ShaderDataType::DVEC4,  ShaderDataType::DVEC4,  DV4, DV4, CR(DV4 * 4, DV4 * 4), CR(DV4 * 4, DV4 * 4)},
    {ShaderDataType::DMAT4x4, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat4x4",      16, ShaderDataType::DVEC4,  ShaderDataType::DVEC4,  DV4, DV4, CR(DV4 * 4, DV4 * 4), CR(DV4 * 4, DV4 * 4)},
    {ShaderDataType::DMAT2x3, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat2x3",      6,  ShaderDataType::DVEC2,  ShaderDataType::DVEC3,  DV4, DV4, CR(DV4 * 2, DV4 * 3), CR(DV4 * 2, DV4 * 3)},
    {ShaderDataType::DMAT2x4, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat2x4",      8,  ShaderDataType::DVEC2,  ShaderDataType::DVEC4,  DV4, DV4, CR(DV4 * 2, DV4 * 4), CR(DV4 * 2, DV4 * 4)},
    {ShaderDataType::DMAT3x2, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat3x2",      6,  ShaderDataType::DVEC3,  ShaderDataType::DVEC2,  DV4, DV4, CR(DV4 * 3, DV4 * 2), CR(DV4 * 3, DV4 * 2)},
    {ShaderDataType::DMAT3x4, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat3x4",      12, ShaderDataType::DVEC3,  ShaderDataType::DVEC4,  DV4, DV4, CR(DV4 * 3, DV4 * 4), CR(DV4 * 3, DV4 * 4)},
    {ShaderDataType::DMAT4x2, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat4x2",      8,  ShaderDataType::DVEC4,  ShaderDataType::DVEC2,  DV4, DV4, CR(DV4 * 4, DV4 * 2), CR(DV4 * 4, DV4 * 2)},
    {ShaderDataType::DMAT4x3, ShaderDataClass::Matrix,  ShaderDataBaseType::Double,   "dmat4x3",      12, ShaderDataType::DVEC4,  ShaderDataType::DVEC3,  DV4, DV4, CR(DV4 * 4, DV4 * 3), CR(DV4 * 4, DV4 * 3)},
    {ShaderDataType::STRUCT,  ShaderDataClass::Struct,  ShaderDataBaseType::Double,   "struct",       0,  ShaderDataType::NONE,   ShaderDataType::NONE,   0,   0,   0,                    0},
  };

  static char const * ShaderResourceType_strings[] = 
  {
    "Invalid Type",
    "sampler2D", "samplerCube"
  };

  ShaderDataType StringToShaderDataType(std::string const& a_type)
  {
    for (uint32_t i = 1; i < static_cast<uint32_t>(ShaderDataType::STRUCT); i++)
    {
      if (a_type == SDT_data[i].str)
        return static_cast<ShaderDataType>(i);
    }
    return ShaderDataType::NONE;
  }

  std::string ShaderDataTypeToString(ShaderDataType a_type)
  {
    return SDT_data[static_cast<uint32_t>(a_type)].str;
  }

  ShaderDataType uint_ToShaderDataType(uint32_t a_val)
  {
    if (a_val <= static_cast<uint32_t>(ShaderDataType::STRUCT))
      return static_cast<ShaderDataType>(a_val);
    return ShaderDataType::NONE;
  }

  uint32_t GetComponentCount(ShaderDataType a_type)
  {
    return SDT_data[static_cast<uint32_t>(a_type)].componentCount;
  }

  uint32_t SizeOfShaderDataType(ShaderDataType a_type)
  {
    uint32_t nElements = GetComponentCount(a_type);
    ShaderDataBaseType baseType = GetShaderDataBaseType(a_type);
    uint32_t baseTypeSize = SizeOfShaderDataBaseType(baseType);
    return baseTypeSize * nElements;
  }

  ShaderDataType GetRowVectorFromMatrix(ShaderDataType a_type)
  {
    uint32_t ind = static_cast<uint32_t>(a_type);
    BSR_ASSERT(SDT_data[ind].dataClass == ShaderDataClass::Matrix, "");
    return SDT_data[ind].matRowType;
  }

  ShaderDataType GetColumnVectorFromMatrix(ShaderDataType a_type)
  {
    uint32_t ind = static_cast<uint32_t>(a_type);
    BSR_ASSERT(SDT_data[ind].dataClass == ShaderDataClass::Matrix, "");
    return SDT_data[ind].matColumnType;
  }

  uint32_t std140BaseAlignmentSingle(ShaderDataType a_type)
  {
    return SDT_data[static_cast<uint32_t>(a_type)].std140BaseAlignmentSingle;
  }

  uint32_t std140BaseAlignmentArray(ShaderDataType a_type)
  {
    return SDT_data[static_cast<uint32_t>(a_type)].std140BaseAlignmentArray;
  }

  uint32_t std140StrideSingle(ShaderDataType a_type, MatrixLayout a_layout)
  {
    uint32_t ind = static_cast<uint32_t>(a_type);
    uint32_t val = SDT_data[ind].std140StrideSingle;
    if (SDT_data[ind].dataClass == ShaderDataClass::Matrix)
    {
      if (a_layout == MatrixLayout::ColumnMajor)
        val = COLUMN(val);
      else
        val = ROW(val);
    }
    return val;
  }

  uint32_t std140StrideArray(ShaderDataType a_type, MatrixLayout a_layout)
  {
    uint32_t ind = static_cast<uint32_t>(a_type);
    uint32_t val = SDT_data[ind].std140StrideArray;
    if (SDT_data[ind].dataClass == ShaderDataClass::Matrix)
    {
      if (a_layout == MatrixLayout::ColumnMajor)
        val = COLUMN(val);
      else
        val = ROW(val);
    }
    return val;
  }

  ShaderDataClass GetShaderDataClass(ShaderDataType a_type)
  {
    return SDT_data[static_cast<uint32_t>(a_type)].dataClass;
  }

  ShaderDataBaseType GetShaderDataBaseType(ShaderDataType a_type)
  {
    return SDT_data[static_cast<uint32_t>(a_type)].baseType;
  }

  uint32_t SizeOfShaderDataBaseType(ShaderDataBaseType a_type)
  {
    static uint32_t const s_sizes[6] = {0, 4, 4, 4, 4, 8};
    return s_sizes[static_cast<uint32_t>(a_type)];
  }

  ShaderResourceType StringToShaderResourceType(std::string const& a_type)
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

  GLenum ShaderDataTypeToOpenGLType(ShaderDataType const a_type)
  {
    static GLenum const s_types[] = 
    {
      GL_INVALID_ENUM,
      GL_BOOL,
      GL_INT,
      GL_UNSIGNED_INT,
      GL_FLOAT,
      GL_DOUBLE,
      GL_BOOL_VEC2,
      GL_BOOL_VEC3,
      GL_BOOL_VEC4,
      GL_INT_VEC2,
      GL_INT_VEC3,
      GL_INT_VEC4,
      GL_UNSIGNED_INT_VEC2,
      GL_UNSIGNED_INT_VEC3,
      GL_UNSIGNED_INT_VEC3,
      GL_FLOAT_VEC2,
      GL_FLOAT_VEC3,
      GL_FLOAT_VEC4,
      GL_DOUBLE_VEC2,
      GL_DOUBLE_VEC3,
      GL_DOUBLE_VEC4,
      GL_FLOAT_MAT2,
      GL_FLOAT_MAT2,
      GL_FLOAT_MAT3,
      GL_FLOAT_MAT3,
      GL_FLOAT_MAT4,
      GL_FLOAT_MAT4,
      GL_FLOAT_MAT2x3,
      GL_FLOAT_MAT2x4,
      GL_FLOAT_MAT3x2,
      GL_FLOAT_MAT3x4,
      GL_FLOAT_MAT4x2,
      GL_FLOAT_MAT4x3,
      GL_DOUBLE_MAT2,
      GL_DOUBLE_MAT2,
      GL_DOUBLE_MAT3,
      GL_DOUBLE_MAT3,
      GL_DOUBLE_MAT4,
      GL_DOUBLE_MAT4,
      GL_DOUBLE_MAT2x3,
      GL_DOUBLE_MAT2x4,
      GL_DOUBLE_MAT3x2,
      GL_DOUBLE_MAT3x4,
      GL_DOUBLE_MAT4x2,
      GL_DOUBLE_MAT4x3,
      GL_INVALID_ENUM
    };
    return s_types[static_cast<uint32_t>(a_type)];
  }

  GLenum ShaderDataBaseTypeToOpenGLType(ShaderDataBaseType a_type)
  {
    static GLenum const s_types[] =
    {
      GL_INVALID_ENUM,
      GL_BOOL,
      GL_INT,
      GL_UNSIGNED_INT,
      GL_FLOAT,
      GL_DOUBLE
    };
    return s_types[static_cast<uint32_t>(a_type)];
  }
}