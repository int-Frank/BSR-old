//@group Messages

#ifndef EN_MESSAGEBUS_H
#define EN_MESSAGEBUS_H

#include <stdint.h>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "MemBuffer.h"
#include "Message.h"
#include "PODArray.h"
#include "Memory.h"

namespace Engine
{
  class Layer;
  class LayerStack;

  class MessageBus
  {
    static size_t const s_bufSize = 1 * 1024 * 1024;
    typedef Core::MemBuffer<s_bufSize, __STDCPP_DEFAULT_NEW_ALIGNMENT__> MemBuffer;
  public:

    MessageBus(LayerStack &);

    //Add message to the queue to be processed at a later time
    void Register(TRef<Message>);

    void DispatchMessages();
    size_t MessageCount();

  private:

    std::mutex              m_mutex;
    std::condition_variable m_cv;
    int                     m_producerIndex;
    std::atomic<size_t>     m_currentlyWriting[2];
    PODArray<Message*>      m_messageQueue[2];
    MemBuffer               m_buf[2];
    LayerStack &            m_layerStack;
  };
}

#endif