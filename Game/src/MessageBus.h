#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <stdint.h>
#include <mutex>

#include "DgDoublyLinkedList.h"
#include "Message.h"

class System;
class SystemStack;

class MessageBus
{

public:

  MessageBus(SystemStack &);

  //Add message to the queue to be processed at a later time
  void Register(Message const &);

  void DispatchMessages();
  size_t MessageCount();

private:

  std::mutex                    m_mutex;       
  Dg::DoublyLinkedList<Message> m_messageQueue;
  SystemStack &                 m_systemStack;
};

#endif