#include "SystemStack.h"
#include "System.h"

SystemStack::SystemStack()
  : m_nextID(1)
{

}

SystemStack::~SystemStack()
{
  Clear();
}

SystemStack::SystemID SystemStack::PushSystem(System * a_system)
{
  m_systemStack.insert(m_nextID, a_system);
  SystemID val = m_nextID;
  m_nextID++;
  return val;
}

void SystemStack::PopSystem(SystemID a_ID)
{
  auto it = m_systemStack.find(a_ID);
  if (it != m_systemStack.end())
  {
    delete it->second;
    m_systemStack.erase(it);
  }
}

void SystemStack::Clear()
{
  for (auto kv : m_systemStack)
    delete kv.second;
  m_systemStack.clear();
}

System * SystemStack::GetSystem(SystemID a_ID)
{
  System * result(nullptr);
  auto it = m_systemStack.find(a_ID);
  if (it != m_systemStack.end())
    result = it->second;
  return result;
}

Dg::Map_AVL<SystemStack::SystemID, System *>::iterator SystemStack::begin()
{
  return m_systemStack.begin();
}

Dg::Map_AVL<SystemStack::SystemID, System *>::iterator SystemStack::end()
{
  return m_systemStack.end();
}