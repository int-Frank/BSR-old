#include "Layer.h"
#include "MessageBus.h"

Layer::Layer(MessageBus * a_pMsgBus)
  : m_pMsgBus(a_pMsgBus)
{

}

Layer::~Layer()
{

}

void Layer::Post(Message const & a_msg)
{
  m_pMsgBus->Register(a_msg);
}

//void Layer::PriorityPost(Message const & a_msg)
//{
//  m_pMsgBus->Dispatch(a_msg);
//}