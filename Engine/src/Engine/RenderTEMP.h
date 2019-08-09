#ifndef RENDERTEMP_H
#define RENDERTEMP_H

#include <stdint.h>
#include "glad/glad.h"

#include "ErrorCodes.h"
#include "DgDynamicArray.h"
#include "DgMap_AVL.h"

enum VertexType
{
  P,
  PT,
  PTN,
  VertexType_COUNT
};

template<VertexType id>
struct Vertex {};

/*
Simple point 2D             - ?? might be handy
Simple line 2D              - debug minimap
Simple polygon 2D           - debug minimap, minimap
*/
template<>
struct Vertex<VertexType::P>
{
  vec3 position;
};

/*
Simple polygon 2D, textured - Menu items, health bar, minimap
*/
template<>
struct Vertex<VertexType::PT>
{
  vec3 position;
  vec2 uv;
};

/*
Game object                 - Walls, enimies, blood decals
*/
template<>
struct Vertex<VertexType::PTN>
{
  vec3 position;
  vec3 normal;
  vec2 uv;
};

//struct Renderable_InGame
//{
//  int nClipPlanes
//  float clipPlanes[4 * 4];
//  ID                    meshID;
//  ID                 texID;
//  ID                matID;
//  ID *                 pStartAffectingLights;
//  uint32_t                  nAffectingLights;
//  mat4                      transform;
//};


class Texture;

class Renderer
{
public:

  template<VertexType id>
  struct Mesh
  {
    Dg::DynamicArray<Vertex<id>> verts;
    Dg::DynamicArray<uint32_t> indices;
  };

  static uint32_t const s_maxBufSze     = 0x10000;

  typedef int32_t ID;

  //User defined class. Useful when you want to clear all data belonging to a particular class.
  //For example, you want to clear all level data.
  typedef int32_t ClassID;

  struct RenderData
  {
    ID shader;
    mat4 transform;
    ID material;
    ID texture;
  };

public:
  
  ID Load(Mesh<VertexType::P> const &, ClassID);
  ID Load(Mesh<VertexType::PT> const &, ClassID);
  ID Load(Mesh<VertexType::PTN> const &, ClassID);
  ID Load(Texture const &, ClassID);

  void BeginScene();
  void Submit(ID, RenderData const &);
  void EndScene();

  void Clear(ClassID);
  void ClearAll();

private:

  struct Record
  {
    VertexType type;    //P, PT, PTN
    uint32_t   vArray;  //Which vertex array is holding the vertex data
    uint32_t   iArray;  //Which index array is holding the vertex data
    uint32_t   ioffset; //index array offset to begin
    uint32_t   iCount;  //number of elements to render
  };

  struct BuffAttr
  {
    Buffer buffer;
    uint32_t currentSize;
  };

  typedef Dg::DynamicArray<BuffAttr> BuffArray;

  Dg::Map_AVL<ClassID, BuffArray>  m_VertexBuffers[VertexType_COUNT];
  Dg::Map_AVL<ClassID, BuffArray>  m_Index_Buffers;

  Dg::Map_AVL<ID, Record>     m_ID_map;

};

#endif