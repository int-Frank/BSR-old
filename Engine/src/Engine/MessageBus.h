#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <stdint.h>
#include <mutex>

#include "DgDoublyLinkedList.h"
#include "Message.h"

namespace Engine
{
  class Layer;
  class LayerStack;

  class MessageBus
  {

  public:

    MessageBus(LayerStack &);

    //Add message to the queue to be processed at a later time
    void Register(Message const &);

    void DispatchMessages();
    size_t MessageCount();

  private:

    std::mutex                    m_mutex;       
    Dg::DoublyLinkedList<Message> m_messageQueue;
    LayerStack &                  m_layerStack;
  };
}

#endif