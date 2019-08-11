#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include "Message.h"
#include "MessageHandler.h"

namespace Engine
{
  void TranslateMessage(Message * dest, Message * source);

  //---IMPL---------------------------------------------------------------------
  namespace impl
  {
    template<int T>
    class MessageWriterSub;

#undef ITEM
#define ITEM(x) template<> class MessageWriterSub<MT_##x>;
    MESSAGE_TYPES;
  }
}

//Use these templates to write your own message writers
#define MESSAGE_WRITER_HEADER(MESSAGE_TYPE) template<>\
    class MessageWriterSub<MESSAGE_TYPE> : public MessageWriter\
    {\
    public:\
      MessageWriterSub(MessageSub<MESSAGE_TYPE> * a_pSrc): m_pSrc(a_pSrc){}\
    private:\
      MessageSub<MESSAGE_TYPE> const * m_pSrc;\
    public:

#define DEFAULT_WRITER(MESSAGE_TYPE) template<>\
    class MessageWriterSub<MESSAGE_TYPE> : public MessageWriter\
    {\
    public:\
      MessageWriterSub(MessageSub<MESSAGE_TYPE> * a_pSrc){}\
    };

#endif