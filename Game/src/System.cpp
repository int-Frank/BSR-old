#include "System.h"
#include "MessageBus.h"

System::System(MessageBus * a_pMsgBus)
  : m_pMsgBus(a_pMsgBus)
{

}

System::~System()
{

}

System::System(System const & a_other)
  : m_pMsgBus(a_other.m_pMsgBus)
{

}

System & System::operator=(System const & a_other)
{
  m_pMsgBus = a_other.m_pMsgBus;
  return *this;
}

System::System(System && a_other)
  : m_pMsgBus(a_other.m_pMsgBus)
{
  a_other.m_pMsgBus = nullptr;
}

System & System::operator=(System && a_other)
{
  if (this != &a_other)
  {
    m_pMsgBus = a_other.m_pMsgBus;
    a_other.m_pMsgBus = nullptr;
  }
  return *this;
}

void System::Post(Message const & a_msg)
{
  m_pMsgBus->Register(a_msg);
}

//void System::PriorityPost(Message const & a_msg)
//{
//  m_pMsgBus->Dispatch(a_msg);
//}