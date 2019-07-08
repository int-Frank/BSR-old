#include "SystemStack.h"
#include "System.h"

void SystemStack::PushSystem(System * a_system)
{
  m_systemStack.push_back(a_system);
}

void SystemStack::PopSystem(System * a_system)
{
  auto it = m_systemStack.begin();
  for (; it != m_systemStack.end(); it++)
  {
    if (*it == a_system)
    {
      delete *it;
      m_systemStack.erase(it);
      break;
    }
  }
}

void SystemStack::Clear()
{
  for (auto ptr : m_systemStack)
    delete ptr;
  m_systemStack.clear();
}

SystemStack::~SystemStack()
{
  Clear();
}

Dg::DoublyLinkedList<System *>::iterator SystemStack::begin()
{
  return m_systemStack.begin();
}

Dg::DoublyLinkedList<System *>::iterator SystemStack::end()
{
  return m_systemStack.end();
}