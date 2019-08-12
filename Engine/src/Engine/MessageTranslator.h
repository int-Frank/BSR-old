#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include "Message.h"
#include "MessageHandler.h"

namespace Engine
{
  void TranslateMessage(Message * dest, Message * source);

  template<int MESSAGE_TYPE>
  class MessageWriter: public MessageHandler
  {
  public:
    MessageWriter(MessageSub<MESSAGE_TYPE> * a_pSrc) {}
  };

}

#endif