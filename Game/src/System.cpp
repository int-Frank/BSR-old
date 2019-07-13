#include "System.h"
#include "MessageBus.h"

System::System(MessageBus * a_pMsgBus)
  : m_pMsgBus(a_pMsgBus)
{

}

System::~System()
{

}

void System::Post(Message const & a_msg)
{
  m_pMsgBus->Register(a_msg);
}

//void System::PriorityPost(Message const & a_msg)
//{
//  m_pMsgBus->Dispatch(a_msg);
//}