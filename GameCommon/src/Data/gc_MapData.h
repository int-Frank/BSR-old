#ifndef GC_MAPDATA_H
#define GC_MAPDATA_H

#include <string>
#include "DgDynamicArray.h"
#include "DgVariableArray2D.h"
#include "DgHyperArray.h"
#include "../gc_Types.h"
#include "../gc_Constants.h"

#define BSR_VERSION 0x10000000

namespace GC
{

  //---------------------------------------------------------------------------------------
  // Vanilla map...
  //---------------------------------------------------------------------------------------



  //---------------------------------------------------------------------------------------
  // BSR formats...
  //---------------------------------------------------------------------------------------




  struct WallTile
  {
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

  struct VisibleTileBlock
  {
    uint32_t x      : 8;
    uint32_t y      : 8;
    uint32_t length : 8;
    uint32_t width  : 8;
  };

  struct AssociatedGeometry
  {
    static uint32_t const None = 0xFFFF;

    uint32_t wallsIndex   : 16;
    uint32_t cornersIndex : 16;
  };

  struct MapData
  {
    Dg::VariableArray2D<VisibleTileBlock>  visibleTileBlocks;
    Dg::HyperArray<uint16_t, 2>            visibleTileMapping;

    Dg::VariableArray2D<uint32_t>          associatedWalls;
    Dg::VariableArray2D<uint32_t>          associatedCorners;
    Dg::HyperArray<AssociatedGeometry, 2>  asociatedGeometryMapping;

    Dg::DynamicArray<Wall>                 Walls;
    Dg::DynamicArray<Corner>               Corners;
  };
}

#endif