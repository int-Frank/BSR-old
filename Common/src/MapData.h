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

class WallFace
{
public:

private:

  vec3 position;
  vec3 direction;
};

class Wall
{
public:

  Wall();

  vec2 GetNormal() const;

  vec3 direction;
  vec3 origin;
  float length;
};

struct Arc
{
  vec3 loa; 
  vec3 roa;
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

  uint32_t  arcQuadrant;
  vec3      point;
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

  uint32_t lowerLeft[2];
  uint32_t dimensions[2];
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
  Dg::DynamicArray<Wall>                  wallGeometryLines;
  Dg::DynamicArray<Corner>                wallGeometryPoints;

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