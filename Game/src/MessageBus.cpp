
#include "Message.h"
#include "MessageBus.h"
#include "System.h"
#include "SystemStack.h"

MessageBus::MessageBus()
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

bool MessageBus::GetNextMessage(Message & a_out)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (m_messageQueue.size() > 0)
  {
    a_out = m_messageQueue.front();
    m_messageQueue.pop_front();
  }
  return m_messageQueue.size() > 0;
}