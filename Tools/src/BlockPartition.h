#ifndef BLOCKPARTITION_H
#define BLOCKPARTITION_H

#include "Types.h"
#include "DgDynamicArray.h"
#include "DgHyperArray.h"
#include "MapData.h"

class BlockPartition
{
private:

  struct XY
  {
    uint32_t x, y;
  };

public:

  BlockPartition(Dg::HyperArray<bool, 2> const &);

  Dg::DynamicArray<Block> Run();

private:

  bool GetNextSeed(XY & currentSeed) const;
  Block GetCandidate(XY const & seed) const;
  void MaskOut(Block const &);
  void CheckCoordinate(XY const & coord) const;

private:

  Dg::HyperArray<bool, 2> m_mask;
};

#endif