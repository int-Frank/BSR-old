#ifndef MAPDATA_H
#define MAPDATA_H

#include "DgDynamicArray.h"
#include "DgVariableArray2D.h"
#include "DgHyperArray.h"
#include "DgMath.h"
#include "Types.h"

#define BSR_VERSION 0x10000000

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

class Wall
{
public:

  Wall();

  enum Direction : uint32_t
  {
    X     = 0,
    Y     = 1,
    negX  = 2,
    negY  = 3,
  };

  vec2 GetNormal() const;

  uint32_t direction;
  vec2 origin;
  float length;

private:

  static vec2 const s_directions[4];
};

struct Arc
{
  vec2 loa;
  vec2 roa;
};

struct Corner
{
  enum ArcQuadrant
  {
    I,
    II,
    III,
    IV
  };

  static Arc const Arcs[4];

  uint32_t quadrant;
  vec2 origin;
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

  enum NodeType : uint32_t
  {
    E_Branch     = 0,
    E_Leaf       = 1,
  };

  NodeType GetType() const;
  uint32_t GetElement() const;
  uint32_t GetChildAboveInd() const;
  uint32_t GetChildBelowInd() const;
  float GetOffset() const;
  uint32_t GetBlockID() const;

  void SetType(NodeType);
  void SetElement(uint32_t);
  void SetChildAboveInd(uint32_t);
  void SetChildBelowInd(uint32_t);
  void SetOffset(float);
  void SetBlockID(uint32_t); //no greater than 31 bits
  
private:

  uint32_t m_data;
  float m_offset;
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