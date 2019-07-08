#ifndef SYSTEMSTACK_H
#define SYSTEMSTACK_H

#include "DgDoublyLinkedList.h"

class System;


class SystemStack
{
public:

  ~SystemStack();

  void PushSystem(System *);
  void PopSystem(System *);
  void Clear();

  Dg::DoublyLinkedList<System *>::iterator begin();
  Dg::DoublyLinkedList<System *>::iterator end();

private:

  Dg::DoublyLinkedList<System *>  m_systemStack;
};

#endif
