
#include "Message.h"
#include "MessageBus.h"
#include "Layer.h"
#include "LayerStack.h"

namespace Engine
{
  MessageBus::MessageBus(LayerStack & a_ss)
    : m_layerStack(a_ss)
  {

  }

  size_t MessageBus::MessageCount()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_messageQueue.size();
  }

  void MessageBus::Register(Message * a_message)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_messageQueue.push_back(a_message);
  } 

  void MessageBus::DispatchMessages()
  {
    while (true)
    {
      Message * pMsg;
      bool shouldBreak = false;
      m_mutex.lock();
      if (m_messageQueue.size() > 0)
      {
        pMsg = m_messageQueue.front();
        m_messageQueue.pop_front();
      }
      else
        shouldBreak = true;
      m_mutex.unlock();

      if (shouldBreak)
        break;

      auto it = m_layerStack.begin();
      for (; it != m_layerStack.end(); it++)
      {
        if (pMsg->Submit(it->second) == MessageHandlerReturnCode::Consumed)
          continue;
      }

      delete pMsg;
    }
  }
}
