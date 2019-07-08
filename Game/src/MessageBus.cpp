
#include "Message.h"
#include "MessageBus.h"
#include "System.h"

MessageBus::MessageBus()
  : m_nextHandle(1)
{

}

size_t MessageBus::MessageCount()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_messageQueue.size();
}

void MessageBus::Register(Message const & a_message)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_messageQueue.push_back(a_message);
} 

void MessageBus::Dispatch(Message const & a_message)
{
  for (auto kv : m_subscribers)
    kv.second->HandleMessage(a_message);
}

bool MessageBus::ProcessNextMessage()
{
  m_mutex.lock();
  if (m_messageQueue.size() > 0)
  {
    Message msg(m_messageQueue.front());
    m_messageQueue.pop_front(); //Leaves the queue to recieve messages from other threads
    m_mutex.unlock();
    Dispatch(msg);
    m_mutex.lock();
  }
  return m_messageQueue.size() > 0;
  m_mutex.unlock();
}

SubscriberHandle MessageBus::RegisterSubscriber(System * a_pSub)
{
  SubscriberHandle handle = m_nextHandle;
  m_subscribers.insert(handle, a_pSub);
  m_nextHandle++;
  return handle;
}

void MessageBus::DeregisterSubscriber(SubscriberHandle a_handle)
{
  m_subscribers.erase(a_handle);
}