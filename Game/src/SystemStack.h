#ifndef SYSTEMSTACK_H
#define SYSTEMSTACK_H

#include "DgMap_AVL.h"

class System;

class SystemStack
{
public:

  typedef unsigned SystemID;

public:

  SystemStack();
  ~SystemStack();

  SystemID PushSystem(System *);
  void PopSystem(SystemID);
  System * GetSystem(SystemID);
  void Clear();

  Dg::Map_AVL<SystemID, System *>::iterator begin();
  Dg::Map_AVL<SystemID, System *>::iterator end();

private:

  Dg::Map_AVL<SystemID, System *>  m_systemStack;
  unsigned                         m_nextID;
};

#endif
