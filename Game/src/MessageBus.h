#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <stdint.h>
#include <mutex>

#include "DgDoublyLinkedList.h"
#include "DgMap_AVL.h"
#include "Message.h"

class System;
typedef uint32_t SubscriberHandle;

class MessageBus
{

public:

  MessageBus();

  //Add message to the queue to be processed at a later time
  void Register(Message const &);

  //Send to all subscribers
  void Dispatch(Message const &);

  SubscriberHandle RegisterSubscriber(System *);
  void DeregisterSubscriber(SubscriberHandle);
  
  //Returns false if no more messages to process
  bool ProcessNextMessage();
  size_t MessageCount();

private:
  std::mutex                                    m_mutex;
                                                
  SubscriberHandle                              m_nextHandle;
  Dg::DoublyLinkedList<Message>                 m_messageQueue;
  Dg::Map_AVL<SubscriberHandle, System *>   m_subscribers;
};

#endif