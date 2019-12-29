//@group Renderer
#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <string>
#include <stdint.h>

namespace Engine
{
  //------------------------------------------------------------------------------------
  // enums...
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

  enum class ShaderResourceType : uint32_t
  {
    NONE        = 0,
    TEXTURE2D,
    TEXTURECUBE
  };

  enum ShaderDomain : int
  {
    SD_INVALID    = -1,
    SD_Vertex     = 0,
    SD_Fragment   = 1,
    SD_Geometry   = 2,
    SD_COUNT
  };

  enum StorageBlockType
  {
    SBT_Uniform,
    SBT_ShaderStorage,
    SBT_COUNT
  };

  //------------------------------------------------------------------------------------
  // Helper functions
  //------------------------------------------------------------------------------------

  uint32_t SizeOfShaderDataType(ShaderDataType);
  ShaderDataType StringToShaderDataType(std::string const&);
  std::string ShaderDataTypeToString(ShaderDataType);
  ShaderDataType uint_ToShaderDataType(uint32_t);
  uint32_t GetComponentCount(ShaderDataType);

  ShaderResourceType StringToShaderResourceType(std::string const&);
  std::string ShaderResourceTypeToString(ShaderResourceType);
}

#endif