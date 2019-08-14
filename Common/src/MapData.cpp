#include "MapData.h"

vec2 const Wall::s_directions[4] = 
{
  {1.0f, 0.0f},
  {0.0f, 1.0f},
  {-1.0f, 0.0f},
  {0.0f, -1.0f}
};

Arc const Corner::Arcs[4] = 
{
  {{0.0f, 1.0f}, {1.0f, 0.0f}},
  {{-1.0f, 0.0f}, {0.0f, 1.0f}},
  {{0.0f, -1.0f}, {-1.0f, 0.0f}},
  {{1.0f, 0.0f}, {0.0f, -1.0f}}
};

Wall::Wall()
  : direction(0)
  , origin(0.0f, 0.0f)
  , length(1.0f)
{

}

vec2 Wall::GetNormal() const
{
  return s_directions[(direction + 3) % 4];
}

Node::Node()
  : m_data(0)
  , m_offset(0.0f)
{

}

Node::Node(Node const & a_other)
  : m_data(a_other.m_data)
  , m_offset(a_other.m_offset)
{

}

Node & Node::operator=(Node const & a_other)
{
  m_data = a_other.m_data;
  m_offset = a_other.m_offset;
  return *this;
}

Node::NodeType Node::GetType() const
{
  return NodeType(m_data & 1);
}

uint32_t Node::GetElement() const
{
  return ((m_data >> 1) & 1);
}

uint32_t Node::GetChildAboveInd() const
{
  return ((m_data >> 2) & 0x7FFF);
}

uint32_t Node::GetChildBelowInd() const
{
  return ((m_data >> 17) & 0x7FFF);
}
 
float Node::GetOffset() const
{
  return m_offset;
}

uint32_t Node::GetBlockID() const
{
  return (m_data & 0x7FFFFFFF);
}

void Node::SetType(NodeType a_val)
{
  m_data = (m_data & 0xFFFFFFFE) | uint32_t(a_val & 1);
}

void Node::SetElement(uint32_t a_val)
{
  m_data = (m_data & 0xFFFFFFFD) | ((a_val & 1) << 1);
}

void Node::SetChildAboveInd(uint32_t a_val)
{
  m_data = (m_data & 0xFFFE0003) | ((a_val & 0x7FFF) << 2);
}

void Node::SetChildBelowInd(uint32_t a_val)
{
  m_data = (m_data & 0x1FFFF) | ((a_val & 0x7FFF) << 17);
}

void Node::SetOffset(float a_val)
{
  m_offset = a_val;
}

void Node::SetBlockID(uint32_t a_val)
{
  m_data = (m_data & 0x80000000) | (a_val & 0x7FFFFFFF);
}