#include "MapData.h"

Node::Node()
  : nodeType(E_Invalid)
{

}

Node::Node(Node const & a_other)
  : nodeType(a_other.nodeType)
{
  switch (nodeType)
  {
    case E_Branch:
    {
      branch = a_other.branch;
      break;
    }
    case E_Leaf:
    {
      leaf = a_other.leaf;
      break;
    }
    default:
    {

    }
  }
}

Node & Node::operator=(Node const & a_other)
{
  if (this != &a_other)
  {
    nodeType = a_other.nodeType;
    switch (nodeType)
    {
      case E_Branch:
      {
        branch = a_other.branch;
        break;
      }
      case E_Leaf:
      {
        leaf = a_other.leaf;
        break;
      }
      default:
      {

      }
    }
  }
  return *this;
}