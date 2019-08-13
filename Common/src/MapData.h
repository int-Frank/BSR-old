#ifndef MAPDATA_H
#define MAPDATA_H

#include "DgDynamicArray.h"
#include "DgVariableArray2D.h"
#include "DgHyperArray.h"
#include "Types.h"

#define BSR_VERSION 0x10000000

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
  enum Element
  {
    X = 0,
    Y = 1,
    W = 0,
    H = 1,
  };

  Block() 
    : lowerLeft{0, 0}
    , dimensions{0, 0}
  {}

  uint8_t lowerLeft[2];
  uint8_t dimensions[2];
};

class Node
{
public:

  Node();
  Node(Node const & a_other);
  Node & operator=(Node const & a_other);

  enum NodeType
  {
    E_Branch     = 0,
    E_Leaf       = 1,
  };

  NodeType Type() const {return NodeType(branch.type);} 
  void SetType(NodeType a_val) {branch.type = a_val;} 

  union
  {
    uint32_t data;

    struct Branch
    {
      unsigned type           : 1;
      unsigned childAboveInd  : 11;
      unsigned childBelowInd  : 11;
      unsigned element        : 1;
      unsigned offset         : 8;
    } branch;

    struct Leaf
    {
      unsigned type     : 1;
      unsigned blockID  : 31;
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