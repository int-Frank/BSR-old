#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <stdint.h>
#include <mutex>

#include "DgDoublyLinkedList.h"
#include "Message.h"

class System;

class MessageBus
{

public:

  MessageBus();

  //Add message to the queue to be processed at a later time
  void Register(Message const &);

  //Returns false if no more messages to process
  bool GetNextMessage(Message &);
  size_t MessageCount();

private:

  std::mutex                    m_mutex;       
  Dg::DoublyLinkedList<Message> m_messageQueue;
};

#endif