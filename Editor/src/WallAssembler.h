#ifndef WALLASSEMBLER_H
#define WALLASSEMBLER_H

#include "Types.h"
#include "MapData.h"
#include "DgDynamicArray.h"
#include "DgHyperArray.h"

class WallAssembler
{
public:

  WallAssembler(Dg::HyperArray<bool, 2> const &);

  void Run();
  Dg::DynamicArray<lineSeg2D> const & GetLines() const;
  Dg::DynamicArray<vec3> const & GetPoints() const;

private:

  void ScanAndInsert(int scanElement, int rowColumn, bool above, int start, int one_past_end);

private:

  WallAssembler(WallAssembler const &);
  WallAssembler & operator=(WallAssembler const &);

private:

  Dg::HyperArray<bool, 2> const &   m_rMask;
  /*
  TODO change lineSeg2D to 
      struct BoundaryLine
      {
        Line    line;
        float   length;
      };
  */
  //TODO Ensure right-perpendicular of the line direction is the outward
  //     normal of the line.
  Dg::DynamicArray<lineSeg2D>       m_lines;

  /*
  TODO change points to

      struct BoundaryPoint
      {
        vec3  origin;
        vec3  loa;
        vec3  roa;
      };
  */
  Dg::DynamicArray<vec3>            m_points;
};

#endif