//@group Messages

#include "Message.h"
#include "MessageBus.h"
#include "Layer.h"
#include "LayerStack.h"

namespace Engine
{
  MessageBus::MessageBus(LayerStack & a_ss)
    : m_layerStack(a_ss)
    , m_producerIndex(0)
    , m_currentlyWriting{0, 0}
  {

  }

  size_t MessageBus::MessageCount()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_messageQueue[m_producerIndex].size();
  }

  void MessageBus::Register(TRef<Message> a_message)
  {
    size_t sze = a_message->Size();
    m_mutex.lock();
    int ind = m_producerIndex;
    m_currentlyWriting[ind]++;
    void * buf = m_buf[ind].Allocate(sze);
    m_messageQueue[ind].push_back(static_cast<Message*>(buf));
    m_mutex.unlock();
    a_message->Clone(buf);
    m_currentlyWriting[ind]--;
  } 

  void MessageBus::DispatchMessages()
  {
    //Save the consumer index
    int ind = m_producerIndex;

    m_mutex.lock();
    m_producerIndex = (m_producerIndex++) % 2;
    m_mutex.unlock();

    if (m_currentlyWriting[ind] != 0)
    {
      std::mutex mut;
      std::unique_lock<std::mutex> lock(mut);
      m_cv.wait(lock, [this, ind = ind]
        {
          return m_currentlyWriting[ind] == 0;
        });
    }
    
    for (size_t i = 0; i < m_messageQueue[ind].size(); i++)
    {
      Message* pMsg = m_messageQueue[ind][i];
      auto it = m_layerStack.begin();
      for (; it != m_layerStack.end(); it++)
      {
        pMsg->Submit(it->second);
        if (pMsg->Is(Message::Flag::Handled))
          break;
      }
    }
    m_messageQueue[ind].clear();
    m_buf[ind].clear();
  }
}
