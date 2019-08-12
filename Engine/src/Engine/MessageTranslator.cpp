#include <cstring>

#include "MessageTranslator.h"
#include "BSR_ASSERT.h"

//Use these templates to write your own message writers
#define MESSAGE_WRITER_HEADER(MESSAGE_TYPE) template<>\
    class MessageWriter<MESSAGE_TYPE> : public MessageHandler\
    {\
    public:\
      MessageWriter(MessageSub<MESSAGE_TYPE> * a_pSrc): m_pSrc(a_pSrc){}\
    private:\
      MessageSub<MESSAGE_TYPE> const * m_pSrc;\
    public:

namespace Engine
{

  MESSAGE_WRITER_HEADER(MT_Input_Text)
    void HandleMessage(MessageSub<MT_GUI_Text> * a_pDest)
    {
      strncpy_s(a_pDest->text, TEXT_INPUT_TEXT_SIZE, m_pSrc->text, TEXT_INPUT_TEXT_SIZE);
    }
  };

  MESSAGE_WRITER_HEADER(MT_Input_KeyUp)
    void HandleMessage(MessageSub<MT_GUI_KeyUp> * a_pDest)
    {
      a_pDest->keyCode = m_pSrc->keyCode;
      a_pDest->modState = m_pSrc->modState;
    }
  };

  MESSAGE_WRITER_HEADER(MT_Input_KeyDown)
    void HandleMessage(MessageSub<MT_GUI_KeyDown> * a_pDest)
    {
      a_pDest->keyCode = m_pSrc->keyCode;
      a_pDest->modState = m_pSrc->modState;
    }
  };

  MESSAGE_WRITER_HEADER(MT_Input_MouseButtonUp)
    void HandleMessage(MessageSub<MT_GUI_MouseButtonUp> * a_pDest)
    {
      a_pDest->button = m_pSrc->button;
      a_pDest->x = m_pSrc->x;
      a_pDest->y = m_pSrc->y;
    }
  };

  MESSAGE_WRITER_HEADER(MT_Input_MouseButtonDown)
    void HandleMessage(MessageSub<MT_GUI_MouseButtonDown> * a_pDest)
    {
      a_pDest->button = m_pSrc->button;
      a_pDest->x = m_pSrc->x;
      a_pDest->y = m_pSrc->y;
    }
  };

  MESSAGE_WRITER_HEADER(MT_Input_MouseMove)
    void HandleMessage(MessageSub<MT_GUI_MouseMove> * a_pDest)
    {
      a_pDest->x = m_pSrc->x;
      a_pDest->y = m_pSrc->y;
    }
  };
  
  class MessageReader : public MessageHandler
  {
  public:

    MessageReader(): m_pWriter(nullptr){}
    virtual ~MessageReader() {delete m_pWriter;}

#undef ITEM
#define ITEM(MESSAGE_TYPE) void HandleMessage(MessageSub<MT_##MESSAGE_TYPE> * a_pMsg)\
    {m_pWriter = new MessageWriter<MT_##MESSAGE_TYPE>(a_pMsg);}
    MESSAGE_TYPES

      MessageHandler * m_pWriter;
  };

  void TranslateMessage(Message * a_pDest, Message * a_pSource)
  {
    MessageReader reader;
    a_pSource->Submit(&reader);
    a_pDest->Submit(reader.m_pWriter);
  }
}