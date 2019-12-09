#ifndef EN_MESSAGE_H
#define EN_MESSAGE_H

#include <stdint.h>
#include <string>
#include <sstream>
#include <type_traits>

#include "core_utils.h"
#include "Memory.h"

#define TEXT_INPUT_TEXT_SIZE 32

namespace Engine
{
#define ITEM()
#define MESSAGE_LIST \
  ITEM(None) \
  ITEM(GoBack) \
  ITEM(GUI_MouseMove) \
  ITEM(GUI_MouseButtonUp) \
  ITEM(GUI_MouseButtonDown) \
  ITEM(GUI_KeyUp) \
  ITEM(GUI_KeyDown) \
  ITEM(GUI_MouseWheelUp) \
  ITEM(GUI_MouseWheelDown) \
  ITEM(GUI_Text) \
  ITEM(Window_Shown) \
  ITEM(Window_Hidden) \
  ITEM(Window_Exposed) \
  ITEM(Window_Moved) \
  ITEM(Window_Resized) \
  ITEM(Window_Size_Changed) \
  ITEM(Window_Minimized) \
  ITEM(Window_Maximized) \
  ITEM(Window_Restored) \
  ITEM(Window_Enter) \
  ITEM(Window_Leave) \
  ITEM(Window_Focus_Gained) \
  ITEM(Window_Focus_Lost) \
  ITEM(Window_Close) \
  ITEM(Window_Take_Focus) \
  ITEM(Input_Text) \
  ITEM(Input_KeyUp) \
  ITEM(Input_KeyDown) \
  ITEM(Input_MouseButtonUp) \
  ITEM(Input_MouseButtonDown) \
  ITEM(Input_MouseWheelUp) \
  ITEM(Input_MouseWheelDown) \
  ITEM(Input_MouseMove) \
  MORE_MESSAGE_TYPES

#undef ITEM
#ifndef MORE_MESSAGE_TYPES
#define MORE_MESSAGE_TYPES
#endif

  //Create enum of all message types
#define ITEM(x) MT_##x,
  enum MessageType : uint32_t
  {
    MESSAGE_LIST
    MT_COUNT
  };

  template<int MESSAGE_TYPE>
  class MessageSub;

  //Forward declare all Message types
#undef ITEM
#define ITEM(MESSAGE_TYPE) template<> class MessageSub<MT_##MESSAGE_TYPE>;
  MESSAGE_LIST
  
  class MessageHandler;

  class Message
  {
  protected:
    uint32_t m_flags;
  public:

    enum class Flag: uint32_t
    {
      Handled = BIT(0),
      Show    = BIT(1)
    };

  public:
    Message(): m_flags(0){}
    bool Is(Flag a_flag) const {return (m_flags & static_cast<uint32_t>(a_flag)) != 0;}
    void SetFlag(Flag a_flag) {m_flags = (m_flags | static_cast<uint32_t>(a_flag));}
    virtual void Submit(MessageHandler *) = 0;
    virtual size_t Size() const = 0;
    virtual TRef<Message> CloneAsTref() const = 0; //Clone at a memory location
    virtual void Clone(void *) const = 0; //Clone at a memory location
    virtual uint32_t GetID() const = 0;
    virtual std::string ToString() const = 0;
  };
  static_assert(std::is_trivially_destructible<Message>::value, "Message must be trivially destructible");

#define MESSAGE_CLASS_HEADER(MESSAGE_TYPE) template<>\
  class MessageSub<MESSAGE_TYPE> : public Message\
  {\
  public:\
    size_t Size() const override {return sizeof(*this);}\
    TRef<Message> CloneAsTref() const override\
    {\
      TRef<MessageSub<MESSAGE_TYPE>> cpy = TRef<MessageSub<MESSAGE_TYPE>>::MakeCopy(this);\
      return StaticPointerCast<Message>(cpy);\
    }\
    void Clone(void * a_buf) const override {new (a_buf) MessageSub<MESSAGE_TYPE>(*this);}\
    void Submit(MessageHandler *) override;\
    uint32_t GetID() const override {return MESSAGE_TYPE;}\
    static uint32_t s_GetID() {return MESSAGE_TYPE;}\
    std::string ToString() const override {return #MESSAGE_TYPE;}

#define MESSAGE_CLASS_HEADER_NO_STRING(MESSAGE_TYPE) template<>\
  class MessageSub<MESSAGE_TYPE> : public Message\
  {\
  public:\
    size_t Size() const override {return sizeof(*this);}\
    TRef<Message> CloneAsTref() const override\
    {\
      TRef<MessageSub<MESSAGE_TYPE>> cpy = TRef<MessageSub<MESSAGE_TYPE>>::MakeCopy(this);\
      return StaticPointerCast<Message>(cpy);\
    }\
    void Clone(void * a_buf) const override {new (a_buf) MessageSub<MESSAGE_TYPE>(*this);}\
    void Submit(MessageHandler *) override;\
    uint32_t GetID() const override {return MESSAGE_TYPE;}\
    static uint32_t s_GetID() {return MESSAGE_TYPE;}

  //-----------------------------------------------------------------------------------
  // Message Classes
  //-----------------------------------------------------------------------------------

  MESSAGE_CLASS_HEADER(MT_None) };
  MESSAGE_CLASS_HEADER(MT_GoBack) };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_GUI_MouseMove)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_GUI_MouseMove [x: " << x << ", y: " << y << "]";
      return ss.str();
    }
    int32_t x;
    int32_t y;
  };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_GUI_MouseButtonUp)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_GUI_MouseButtonUp [button: " << button << ", x: " << x << ", y: " << y << "]";
      return ss.str();
    }
    uint32_t  button;
    int32_t   x;
    int32_t   y;
  };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_GUI_MouseButtonDown)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_GUI_MouseButtonDown [button: " << button << ", x: " << x << ", y: " << y << "]";
      return ss.str();
    }
    uint32_t  button;
    int32_t   x;
    int32_t   y;
  };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_GUI_KeyUp)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_GUI_KeyUp [key: " << keyCode << ", modState: " << modState << "]";
      return ss.str();
    }
    uint32_t  keyCode;
    uint16_t  modState;
  };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_GUI_KeyDown)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_GUI_KeyDown [key: " << keyCode << ", modState: " << modState << "]";
      return ss.str();
    }
    uint32_t  keyCode;
    uint16_t  modState;
  };

  MESSAGE_CLASS_HEADER(MT_GUI_MouseWheelUp) };
  MESSAGE_CLASS_HEADER(MT_GUI_MouseWheelDown) };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_GUI_Text)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_GUI_Text [text: " << text << "]";
      return ss.str();
    }
    char text[TEXT_INPUT_TEXT_SIZE];
  };

  MESSAGE_CLASS_HEADER(MT_Window_Shown) };
  MESSAGE_CLASS_HEADER(MT_Window_Hidden) };
  MESSAGE_CLASS_HEADER(MT_Window_Exposed) };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_Window_Moved)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_Window_Moved [x: " << x << ", y: " << y << "]";
      return ss.str();
    }
    int32_t   x;
    int32_t   y;
  };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_Window_Resized)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_Window_Resized [w: " << w << ", h: " << h << "]";
      return ss.str();
    }
    int32_t   w;
    int32_t   h;
  };

  MESSAGE_CLASS_HEADER_NO_STRING(MT_Window_Size_Changed)
    std::string ToString() const 
    {
      std::stringstream ss;
      ss << "MT_Window_Size_Changed [w: " << w << ", h: " << h << "]";
      return ss.str();
    }
    int32_t   w;
    int32_t   h;
  };

  MESSAGE_CLASS_HEADER(MT_Window_Minimized) };
  MESSAGE_CLASS_HEADER(MT_Window_Maximized) };
  MESSAGE_CLASS_HEADER(MT_Window_Restored) };
  MESSAGE_CLASS_HEADER(MT_Window_Enter) };
  MESSAGE_CLASS_HEADER(MT_Window_Leave) };
  MESSAGE_CLASS_HEADER(MT_Window_Focus_Gained) };
  MESSAGE_CLASS_HEADER(MT_Window_Focus_Lost) };
  MESSAGE_CLASS_HEADER(MT_Window_Close) };
  MESSAGE_CLASS_HEADER(MT_Window_Take_Focus) };

  MESSAGE_CLASS_HEADER(MT_Input_Text)
    char text[TEXT_INPUT_TEXT_SIZE];
  };

  MESSAGE_CLASS_HEADER(MT_Input_KeyUp)
    uint32_t  keyCode;
    uint16_t   modState;
  };

  MESSAGE_CLASS_HEADER(MT_Input_KeyDown)
    uint32_t  keyCode;
    uint16_t   modState;
  };

  MESSAGE_CLASS_HEADER(MT_Input_MouseButtonUp)
    uint32_t  button;
    int32_t    x;
    int32_t    y;
  };

  MESSAGE_CLASS_HEADER(MT_Input_MouseButtonDown)
    uint32_t  button;
    int32_t    x;
    int32_t    y;
  };

  MESSAGE_CLASS_HEADER(MT_Input_MouseWheelUp) };
  MESSAGE_CLASS_HEADER(MT_Input_MouseWheelDown) };

  MESSAGE_CLASS_HEADER(MT_Input_MouseMove)
    int32_t x;
    int32_t y;
  };

  //-----------------------------------------------------------------------------------
  // Make sure messages are trivially destructable
  //-----------------------------------------------------------------------------------
#undef ITEM
#define ITEM(MESSAGE_TYPE) static_assert(std::is_trivially_destructible<MessageSub<MESSAGE_TYPE>>::value, #MESSAGE_TYPE " must be trivially destructible");\
  MESSAGE_LIST

  //-----------------------------------------------------------------------------------
  // Message translators
  //-----------------------------------------------------------------------------------
  
  namespace MessageTranslator
  {
    typedef void (*MessageTranslatorFn)(Message * dest, Message const * src);

    void Clear();
    bool Exists(MessageType, MessageType);
    void AddTranslator(MessageType, MessageType, MessageTranslatorFn); //Will override
    void Translate(Message * dest, Message const * src);
    void AddDefaultTranslators();
  }
}
#endif