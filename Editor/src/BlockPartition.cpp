#include <exception>
#include <sstream>

#include "BlockPartition.h"

//TODO Implement a maximum block size. Blocks above this size should be broken down.


void BlockPartition::CheckCoordinate(XY const & a_coord) const
{
  if (a_coord.x >= m_mask.length(0) || a_coord.y >= m_mask.length(1))
  {
    std::stringstream ss;
    ss << "Coordinate [" << a_coord.x << ", " << a_coord.y << "] out of range. Mask dimensions are ["
      << m_mask.length(0) << ", " << m_mask.length(1) << "]";
    throw std::out_of_range(ss.str());
  }
}

bool BlockPartition::GetNextSeed(XY & a_out) const
{
  CheckCoordinate(a_out);

  for (; a_out.y < uint32_t(m_mask.length(1)); a_out.y++)
  {
    for (; a_out.x < uint32_t(m_mask.length(0)); a_out.x++)
    {
      if (!m_mask.at(a_out.x, a_out.y))
      {
        return true;
      }
    }
    a_out.x = 0;
  }
  return false;
}

Block BlockPartition::GetCandidate(XY const & a_seed) const
{
  CheckCoordinate(a_seed);

  if (m_mask(a_seed.x, a_seed.y))
  {
    std::stringstream ss;
    ss << "GetCandidate(): Coordinate [" << a_seed.x << ", " << a_seed.y << "] is not a proper seed";
    throw std::exception(ss.str().c_str());
  }

  uint32_t h = 1;
  uint32_t w = 1;
  uint32_t min_x = a_seed.x;
  uint32_t min_y = a_seed.y;

  uint32_t flags = 0;
  while (flags != 0xF)
  {
    //Try to expand block in the -x direction
    if (!(flags & 1))
    {
      if (min_x > 0)
      {
        for (uint32_t y = min_y; y < min_y + h; y++)
        {
          if (m_mask.at(size_t(min_x - 1), size_t(y)))
          {
            flags |= 1;
            break;
          }
        }

        if (!(flags & 1))
        {
          min_x = min_x - 1;
          w = w + 1;
        }
      }
      else
      {
        flags |= 1;
      }
    }

    //Try to expand block in the +y direction
    if (!(flags & 2))
    {
      if (min_y + h < m_mask.length(1))
      {
        for (uint32_t x = min_x; x < min_x + w; x++)
        {
          if (m_mask.at(size_t(x), size_t(min_y + h)))
          {
            flags |= 2;
            break;
          }
        }

        if (!(flags & 2))
        {
          h = h + 1;
        }
      }
      else
      {
        flags |= 2;
      }
    }

    //Try to expand block in the +x direction
    if (!(flags & 4))
    {
      if (min_x + w < m_mask.length(0))
      {
        for (uint32_t y = min_y; y < min_y + h; y++)
        {
          if (m_mask.at(size_t(min_x + w), size_t(y)))
          {
            flags |= 4;
            break;
          }
        }

        if (!(flags & 4))
        {
          w = w + 1;
        }
      }
      else
      {
        flags |= 4;
      }
    }

    //Try to expand block in the -y direction
    if (!(flags & 8))
    {
      if (min_y > 0)
      {
        for (uint32_t x = min_x; x < min_x + w; x++)
        {
          if (m_mask.at(size_t(x), size_t(min_y - 1)))
          {
            flags |= 8;
            break;
          }
        }

        if (!(flags & 8))
        {
          min_y = min_y - 1;
          h = h + 1;
        }
      }
      else
      {
        flags |= 8;
      }
    }
  }

  Block result;

  result.lowerLeft[0] = float(min_x);
  result.lowerLeft[1] = float(min_y);
  result.dimensions[0] = float(w);
  result.dimensions[1] = float(h);

  return result;
}

void BlockPartition::MaskOut(Block const & a_block)
{
  //TODO add range checks

  for (size_t x = size_t(a_block.lowerLeft[0]); x < size_t(a_block.lowerLeft[0] + a_block.dimensions[0]); x++)
  {
    for (size_t y = size_t(a_block.lowerLeft[1]); y < size_t(a_block.lowerLeft[1] + a_block.dimensions[1]); y++)
    {
      m_mask(x, y) = true;
    }
  }
}

BlockPartition::BlockPartition(Dg::HyperArray<bool, 2> const & m_mask)
  : m_mask(m_mask)
{

}

Dg::DynamicArray<Block> BlockPartition::Run()
{
  Dg::DynamicArray<Block> result;
  XY seed{0, 0};

  while (GetNextSeed(seed))
  {
    Block block = GetCandidate(seed);
    MaskOut(block);
    result.push_back(block);
  }
  return result;
}