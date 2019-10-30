#ifndef EN_MESSAGEHANDLER_H
#define EN_MESSAGEHANDLER_H

#include "Message.h"

namespace Engine
{
  class MessageHandler
  {
    
  public:

    virtual ~MessageHandler() = default;

#undef ITEM
#define ITEM(x) virtual void HandleMessage(MessageSub<MT_##x> *) {}
    MESSAGE_LIST;
  };
}

#endif