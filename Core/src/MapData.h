#ifndef MAPDATA_H
#define MAPDATA_H

#include "DgDynamicArray.h"
#include "DgVariableArray2D.h"
#include "DgHyperArray.h"
#include "Types.h"

#define BSR_VERSION 0x10000000

typedef uint32_t BlockID; //This is simply the index to the block in m_rBlocks

struct WallPhysics
{
  vec4  p0;     //Start point of the wall
  vec4  n;      //normal to the line
  float length; //wall length
};

struct Object
{
  enum Class
  {
    E_Other,

    //Structural     
    E_Wall,         
    E_Floor,
    E_Ceiling,
    E_Door,

    //Object
    E_Static,         //SOLID / SHOOTABLE
    E_Destructable,
    E_ActorAllDir,
    E_ActorOneDir,
    E_ActorSpawnIn,  //Pod alien, electro alien...
    E_Collectable,
    E_PushWall
  };

  //        class         |        flags        |     
  // 0000 0000  0000 0000   0000 0000  0000 0000
  //class flags could be: moving/stationary, spawns at what difficulty level...
  uint32_t    ID;
  vec4        position;
  vec4        rotation; 
};

struct Block
{
  vec2 lowerLeft;  //[x, y]
  vec2 dimensions; //[w, h]
};

class Node
{
public:

  Node();
  Node(Node const & a_other);
  Node & operator=(Node const & a_other);

  enum
  {
    //Node type
    E_Branch     = 0, //Axis Aligned
    E_Leaf       = 1,
    E_Invalid    = -1,
  };

  int nodeType;
  union
  {
    struct Branch
    {
      uint16_t      child_ABOVE_ind;
      uint16_t      child_BELOW_ind;
      uint16_t      element;
      float         offset;
    } branch;

    struct Leaf
    {
      BlockID blockID;
    } leaf;
  };
};

/*
Built in game:
  Dg::VariableArray2D<uint32_t>           PVSStaticObjectGeometry;  //Built in game
  Dg::DynamicArray<rect3D>                wallGeometryMasterList3D; //Built in game
  Dg::DynamicArray<uint32_t>              staticObjectGeometryMasterList3D; //Built in game
  Dg::DynamicArray<disk2D>                staticObjectGeometryMasterList2D; //Built in game
*/

struct MapFileData
{
  Dg::DynamicArray<Node>                  bspTree;                 

  //Each block will have a corresponding list of visible blocks, objects and geometry
  Dg::DynamicArray<Block>                 blocks;
  Dg::VariableArray2D<uint32_t>           PVSBlocks;
  Dg::VariableArray2D<uint32_t>           PVSObjects;
  Dg::VariableArray2D<uint32_t>           blockGeometryLines;
  Dg::VariableArray2D<uint32_t>           blockGeometryPoints;

  //Includes all sprites; actors; wall, floor and ceiling tiles
  Dg::DynamicArray<Object>                objectMasterList; 
  Dg::DynamicArray<lineSeg2D>             wallGeometryLines;
  Dg::DynamicArray<vec3>                  wallGeometryPoints;

  //These can be loaded directly from MAPTEMP.BS6
  //Dg::HyperArray<uint16_t, 2>             floorPlanObject;
  //Dg::HyperArray<uint16_t, 2>             floorPlanStructural;

  uint16_t                                floorTile;
  uint16_t                                ceilingTile;
};

struct EpisodeData
{
  
};

#endif