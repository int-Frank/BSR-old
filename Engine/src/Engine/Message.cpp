#include "Message.h"
#include "MessageHandler.h"

#undef ITEM
#define ITEM(x) MessageHandlerReturnCode MessageSub<MT_##x>::Submit(MessageHandler * a_pHandler)\
    {return a_pHandler->HandleMessage(static_cast<MessageSub<MT_##x> *>(this));}\

namespace Engine
{
  MESSAGE_TYPES
}