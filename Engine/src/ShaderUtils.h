//@group Renderer
#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <string>
#include <stdint.h>

#define SD32(x) static_cast<uint32_t>(::Engine::ShaderDomain::x)
#define SDT32(x) static_cast<uint32_t>(::Engine::ShaderDataType::x)
#define SRT32(x) static_cast<uint32_t>(::Engine::ShaderResourceType::x)
#define SBT32(x) static_cast<uint32_t>(::Engine::StorageBlockType::x)

namespace Engine
{
  //------------------------------------------------------------------------------------
  // Generic ...
  //------------------------------------------------------------------------------------
  enum class ShaderDataType : uint32_t
  {
    NONE     = 0,

    BOOL,
    INT, UINT,
    FLOAT, DOUBLE,

    BVEC2, BVEC3, BVEC4,
    IVEC2, IVEC3, IVEC4,
    UVEC2, UVEC3, UVEC4,
    VEC2, VEC3, VEC4,
    DVEC2, DVEC3, DVEC4,

    MAT2, MAT2x2, MAT3, MAT3x3, MAT4, MAT4x4,
    MAT2x3, MAT2x4,
    MAT3x2, MAT3x4,
    MAT4x2, MAT4x3,

    DMAT2, DMAT2x2, DMAT3, DMAT3x3, DMAT4, DMAT4x4,
    DMAT2x3, DMAT2x4,
    DMAT3x2, DMAT3x4,
    DMAT4x2, DMAT4x3,

    STRUCT
  };

  enum class ShaderDataClass : uint32_t
  {
    None,
    Scalar,
    Vector,
    Matrix,
    Struct
  };

  enum class MatrixLayout : uint32_t
  {
    RowMajor,
    ColumnMajor
  };

  enum class ShaderDataBaseType : uint32_t
  {
    None,
    Bool,
    Int,
    UInt,
    Float,
    Double
  };

  enum class ShaderResourceType : uint32_t
  {
    NONE        = 0,
    TEXTURE2D,
    TEXTURECUBE
  };

  enum class ShaderDomain : uint32_t
  {
    INVALID    = 0xFFFFFFFF,
    Vertex     = 0,
    Fragment   = 1,
    Geometry   = 2,
    COUNT
  };

  enum class StorageBlockType : uint32_t
  {
    Uniform,
    ShaderStorage,
    COUNT
  };


  ShaderDataType StringToShaderDataType(std::string const&);
  std::string ShaderDataTypeToString(ShaderDataType);
  ShaderDataType uint_ToShaderDataType(uint32_t);
  uint32_t GetComponentCount(ShaderDataType);
  uint32_t SizeOfShaderDataType(ShaderDataType);

  ShaderDataType GetRowVectorFromMatrix(ShaderDataType);
  ShaderDataType GetColumnVectorFromMatrix(ShaderDataType);

  uint32_t std140BaseAlignmentSingle(ShaderDataType);
  uint32_t std140BaseAlignmentArray(ShaderDataType);
  uint32_t std140StrideSingle(ShaderDataType, MatrixLayout layout = MatrixLayout::ColumnMajor);
  uint32_t std140StrideArray(ShaderDataType, MatrixLayout layout = MatrixLayout::ColumnMajor);

  ShaderDataClass GetShaderDataClass(ShaderDataType);
  ShaderDataBaseType GetShaderDataBaseType(ShaderDataType);

  uint32_t SizeOfShaderDataBaseType(ShaderDataBaseType);

  ShaderResourceType StringToShaderResourceType(std::string const&);
  std::string ShaderResourceTypeToString(ShaderResourceType);

  //------------------------------------------------------------------------------------
  // OpenGL specific
  //------------------------------------------------------------------------------------

  typedef unsigned int GLenum;

  GLenum ShaderDataTypeToOpenGLType(ShaderDataType);
  GLenum ShaderDataBaseTypeToOpenGLType(ShaderDataBaseType);
  GLenum ShaderDomainToOpenGLType(ShaderDomain);

}

#endif