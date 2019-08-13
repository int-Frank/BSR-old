#include "MapData.h"

Node::Node()
  : data(0)
{

}

Node::Node(Node const & a_other)
  : data(a_other.data)
{

}

Node & Node::operator=(Node const & a_other)
{
  data = a_other.data;
  return *this;
}