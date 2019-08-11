#include <cstring>

#include "MessageTranslator.h"
#include "BSR_ASSERT.h"

namespace Engine
{
  namespace impl
  {
    class MessageWriter : public MessageHandler
    {
    public:

#undef ITEM
#define ITEM(x) virtual MessageHandlerReturnCode HandleMessage(MessageSub<MT_##x> *a_pMsg) {return MessageHandlerReturnCode::None;}
      MESSAGE_TYPES;
    };

    class MessageReader : public MessageHandler
    {
    public:

      MessageReader(): m_pWriter(nullptr){}
      virtual ~MessageReader() {delete m_pWriter;}

#undef ITEM
#define ITEM(x) MessageHandlerReturnCode HandleMessage(MessageSub<MT_##x> *);
      MESSAGE_TYPES;

      MessageWriter * m_pWriter;
    };

    DEFAULT_WRITER(MT_None);
    DEFAULT_WRITER(MT_GoBack);
    DEFAULT_WRITER(MT_GUI_MouseMove);
    DEFAULT_WRITER(MT_GUI_MouseButtonUp);
    DEFAULT_WRITER(MT_GUI_MouseButtonDown);
    DEFAULT_WRITER(MT_GUI_KeyUp);
    DEFAULT_WRITER(MT_GUI_KeyDown);
    DEFAULT_WRITER(MT_GUI_MouseWheelUp);
    DEFAULT_WRITER(MT_GUI_MouseWheelDown);
    DEFAULT_WRITER(MT_GUI_Text);
    DEFAULT_WRITER(MT_Window_Shown);
    DEFAULT_WRITER(MT_Window_Hidden);
    DEFAULT_WRITER(MT_Window_Exposed);
    DEFAULT_WRITER(MT_Window_Moved);
    DEFAULT_WRITER(MT_Window_Resized);
    DEFAULT_WRITER(MT_Window_Size_Changed);
    DEFAULT_WRITER(MT_Window_Minimized);
    DEFAULT_WRITER(MT_Window_Maximized);
    DEFAULT_WRITER(MT_Window_Restored);
    DEFAULT_WRITER(MT_Window_Enter);
    DEFAULT_WRITER(MT_Window_Leave);
    DEFAULT_WRITER(MT_Window_Focus_Gained);
    DEFAULT_WRITER(MT_Window_Focus_Lost);
    DEFAULT_WRITER(MT_Window_Close);
    DEFAULT_WRITER(MT_Window_Take_Focus);

    MESSAGE_WRITER_HEADER(MT_Input_Text)
      MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_Text> * a_pDest)
      {
        strncpy_s(a_pDest->text, TEXT_INPUT_TEXT_SIZE, m_pSrc->text, TEXT_INPUT_TEXT_SIZE);
        return MessageHandlerReturnCode::None;
      }
    };

    MESSAGE_WRITER_HEADER(MT_Input_KeyUp)
      MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_KeyUp> * a_pDest)
      {
        a_pDest->keyCode = m_pSrc->keyCode;
        a_pDest->modState = m_pSrc->modState;
        return MessageHandlerReturnCode::None;
      }
    };

    MESSAGE_WRITER_HEADER(MT_Input_KeyDown)
      MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_KeyDown> * a_pDest)
      {
        a_pDest->keyCode = m_pSrc->keyCode;
        a_pDest->modState = m_pSrc->modState;
        return MessageHandlerReturnCode::None;
      }
    };

    MESSAGE_WRITER_HEADER(MT_Input_MouseButtonUp)
      MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseButtonUp> * a_pDest)
      {
        a_pDest->button = m_pSrc->button;
        a_pDest->x = m_pSrc->x;
        a_pDest->y = m_pSrc->y;
        return MessageHandlerReturnCode::None;
      }
    };

    MESSAGE_WRITER_HEADER(MT_Input_MouseButtonDown)
      MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseButtonDown> * a_pDest)
      {
        a_pDest->button = m_pSrc->button;
        a_pDest->x = m_pSrc->x;
        a_pDest->y = m_pSrc->y;
        return MessageHandlerReturnCode::None;
      }
    };

    DEFAULT_WRITER(MT_Input_MouseWheelUp);
    DEFAULT_WRITER(MT_Input_MouseWheelDown);

    MESSAGE_WRITER_HEADER(MT_Input_MouseMove)
      MessageHandlerReturnCode HandleMessage(MessageSub<MT_GUI_MouseMove> * a_pDest)
      {
        a_pDest->x = m_pSrc->x;
        a_pDest->y = m_pSrc->y;
        return MessageHandlerReturnCode::None;
      }
    };

#undef ITEM
#define ITEM(MESSAGE_TYPE) MessageHandlerReturnCode MessageReader::HandleMessage(MessageSub<MT_##MESSAGE_TYPE> * a_pMsg)\
  {m_pWriter = new MessageWriterSub<MT_##MESSAGE_TYPE>(a_pMsg); return MessageHandlerReturnCode::None;}
    MESSAGE_TYPES
  
  }

  void TranslateMessage(Message * a_pDest, Message * a_pSource)
  {
    impl::MessageReader reader;
    a_pSource->Submit(&reader);
    a_pDest->Submit(reader.m_pWriter);
  }
}