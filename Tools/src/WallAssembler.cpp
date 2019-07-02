#include "WallAssembler.h"

WallAssembler::WallAssembler(Dg::HyperArray<bool, 2> const & a_mask)
  : m_rMask(a_mask)
{

}

Dg::DynamicArray<lineSeg2D> const & WallAssembler::GetLines() const
{
  return m_lines;
}

Dg::DynamicArray<vec3> const & WallAssembler::GetPoints() const
{
  return m_points;
}

void WallAssembler::ScanAndInsert(int a_scanElement,
                                  int a_rowCol,
                                  bool a_above,
                                  int a_start, 
                                  int a_end)
{
  //TODO add checks
  float foffset = a_above ? 1.0f : 0.0f;
  int ioffset = a_above ? 1 : -1;

  size_t xy[2];
  xy[(a_scanElement + 1) % 2] = size_t(a_rowCol);

  size_t i = size_t(a_start);
  while (i < size_t(a_end))
  {
    xy[a_scanElement] = i;

    //No wall
    if (!m_rMask(xy[0], xy[1]))
    {
      //Wall on the offset
      size_t xyWall[2] = {xy[0], xy[1]};
      xyWall[(a_scanElement + 1) % 2] = a_rowCol + ioffset;
      if (m_rMask(xyWall[0], xyWall[1]))
      {
        vec3 p0;
        p0[a_scanElement] = float(xy[a_scanElement]);
        p0[(a_scanElement + 1) % 2] = float(xy[(a_scanElement + 1) % 2]) + foffset;
        p0[2] = 1.0f;

        while (!m_rMask(xy[0], xy[1]) 
             && m_rMask(xyWall[0], xyWall[1]) 
             && i < size_t(a_end))
        {
          i++;
          xy[a_scanElement] = i;
          xyWall[a_scanElement] = i;
        }

        vec3 p1;
        p1[a_scanElement] = float(xy[a_scanElement]);
        p1[(a_scanElement + 1) % 2] = float(xy[(a_scanElement + 1) % 2]) + foffset;
        p1[2] = 1.0f;

        lineSeg2D line(p0, p1);
        m_lines.push_back(line);
      }
    }
    i++;
  }
}

void WallAssembler::Run()
{
  m_lines.clear();
  m_points.clear();

  //Add in lines
  for (size_t y = 1; y < m_rMask.length(1) - 1; y++)
  {
    ScanAndInsert(0, (int)y, true, 1, (int)m_rMask.length(0));
    ScanAndInsert(0, (int)y, false, 1, (int)m_rMask.length(0));
  }

  for (size_t x = 1; x < m_rMask.length(0) - 1; x++)
  {
    ScanAndInsert(1, (int)x, true, 1, (int)m_rMask.length(1));
    ScanAndInsert(1, (int)x, false, 1, (int)m_rMask.length(1));
  }

  //Add in points
  for (size_t y = 1; y < m_rMask.length(1) - 1; y++)
  {
    for (size_t x = 1; x < m_rMask.length(0) - 1; x++)
    {
      if (m_rMask(x, y))
      {
        bool freePt = m_rMask(x - 1, y);
        freePt = freePt && m_rMask(x - 1, y + 1);
        freePt = freePt && m_rMask(x, y + 1);
        if (freePt)
        {
          vec3 pt;
          pt[0] = float(x);
          pt[1] = float(y + 1);
          pt[2] = 1.0f;
          m_points.push_back(pt);
        }

        freePt = m_rMask(x, y + 1);
        freePt = freePt && m_rMask(x + 1, y + 1);
        freePt = freePt && m_rMask(x + 1, y);
        if (freePt)
        {
          vec3 pt;
          pt[0] = float(x + 1);
          pt[1] = float(y + 1);
          pt[2] = 1.0f;
          m_points.push_back(pt);
        }

        freePt = m_rMask(x + 1, y);
        freePt = freePt && m_rMask(x + 1, y - 1);
        freePt = freePt && m_rMask(x, y - 1);
        if (freePt)
        {
          vec3 pt;
          pt[0] = float(x + 1);
          pt[1] = float(y);
          pt[2] = 1.0f;
          m_points.push_back(pt);
        }

        freePt = m_rMask(x, y - 1);
        freePt = freePt && m_rMask(x - 1, y - 1);
        freePt = freePt && m_rMask(x - 1, y);
        if (freePt)
        {
          vec3 pt;
          pt[0] = float(x);
          pt[1] = float(y);
          pt[2] = 1.0f;
          m_points.push_back(pt);
        }
      }
    }
  }
}