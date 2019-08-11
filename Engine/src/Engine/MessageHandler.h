#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "Message.h"

namespace Engine
{
  class MessageHandler
  {
    
  public:

    virtual ~MessageHandler() = default;

#undef ITEM
#define ITEM(x) virtual MessageHandlerReturnCode HandleMessage(MessageSub<MT_##x> *) {return MessageHandlerReturnCode::None;}
    MESSAGE_TYPES;
  };
}

#endif