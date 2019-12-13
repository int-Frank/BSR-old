//@group Layers

#include "Layer.h"
#include "MessageBus.h"

namespace Engine
{
  Layer::Layer(MessageBus * a_pMsgBus)
    : m_pMsgBus(a_pMsgBus)
  {

  }

  Layer::~Layer()
  {

  }

  void Layer::Post(TRef<Message> a_msg)
  {
    m_pMsgBus->Register(a_msg);
  }
}