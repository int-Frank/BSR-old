#ifndef BLOCKPARTITION_H
#define BLOCKPARTITION_H

#include "Types.h"
#include "DgDynamicArray.h"
#include "DgHyperArray.h"
#include "MapData.h"

//TODO  Look for large blocks first (10 x 10 for exmaple). This way we can ensure larger areas are not
//      split by smaller ones. We can the decrement and search again (9 x 9 for example) all the way to 1.
class BlockPartition
{
private:

  struct XY
  {
    uint32_t x, y;
  };

public:

  BlockPartition(Dg::HyperArray<bool, 2> const &);
  void SetMaxBlockSize(unsigned);
  Dg::DynamicArray<Block> Run();

private:

  bool GetNextSeed(XY & currentSeed) const; //change to 'bool GetNextSeed(XY & currentSeed, int size) const;'
  Block GetCandidate(XY const & seed) const;
  void MaskOut(Block const &);
  void CheckCoordinate(XY const & coord) const;

private:

  Dg::HyperArray<bool, 2> m_mask;
  unsigned                m_maxBlockSize;
};

#endif