//@group Messages

#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <string>
#include <type_traits>

#include "core_utils.h"
#include "Memory.h"

#define TEXT_INPUT_TEXT_SIZE 32

//Smart list reduces the amount of code we have to write
#define ITEM()
#define MESSAGE_LIST \
  ITEM(None, None) \
  ITEM(GoBack, None) \
  ITEM(GUI_MouseMove, GUI) \
  ITEM(GUI_MouseButtonUp, GUI) \
  ITEM(GUI_MouseButtonDown, GUI) \
  ITEM(GUI_KeyUp, GUI) \
  ITEM(GUI_KeyDown, GUI) \
  ITEM(GUI_MouseWheelUp, GUI) \
  ITEM(GUI_MouseWheelDown, GUI) \
  ITEM(GUI_Text, GUI) \
  ITEM(Window_Shown, Window) \
  ITEM(Window_Hidden, Window) \
  ITEM(Window_Exposed, Window) \
  ITEM(Window_Moved, Window) \
  ITEM(Window_Resized, Window) \
  ITEM(Window_Size_Changed, Window) \
  ITEM(Window_Minimized, Window) \
  ITEM(Window_Maximized, Window) \
  ITEM(Window_Restored, Window) \
  ITEM(Window_Enter, Window) \
  ITEM(Window_Leave, Window) \
  ITEM(Window_Focus_Gained, Window) \
  ITEM(Window_Focus_Lost, Window) \
  ITEM(Window_Close, Window) \
  ITEM(Window_Take_Focus, Window) \
  ITEM(Input_Text, Input) \
  ITEM(Input_KeyUp, Input) \
  ITEM(Input_KeyDown, Input) \
  ITEM(Input_MouseButtonUp, Input) \
  ITEM(Input_MouseButtonDown, Input) \
  ITEM(Input_MouseWheelUp, Input) \
  ITEM(Input_MouseWheelDown, Input) \
  ITEM(Input_MouseMove, Input)

namespace Engine
{
  //General Message command
  typedef void(*MessageCommandFn)(void*);

  enum MessageCategory : uint32_t
  {
    MC_None    = 0,
    MC_GUI,
    MC_Input,
    MC_Window,
    MC_CLIENT_BEGIN //Create your own categories beginning at this value
  };

  class Message
  {
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
    virtual uint32_t GetID() const = 0;
    uint32_t GetCategory() const;
    virtual size_t Size() const = 0;
    virtual TRef<Message> CloneAsTRef() const = 0; //Clone at a memory location
    virtual void Clone(void *) const = 0; //Clone at a memory location
    virtual std::string ToString() const = 0;

  protected:
    uint32_t m_flags;
  protected:
    static uint32_t GetNewID(uint32_t a_class);
  };
  static_assert(std::is_trivially_destructible<Message>::value, "Message must be trivially destructible");

#define MESSAGE_CLASS_HEADER(MESSAGE_TYPE) class Message_##MESSAGE_TYPE : public Message\
  {\
    static uint32_t s_ID;\
  public:\
    static uint32_t GetStaticID();\
    uint32_t GetID() const override;\
    size_t Size() const override;\
    TRef<Message> CloneAsTRef() const override;\
    void Clone(void * a_buf) const override;\
    std::string ToString() const override;

  //-----------------------------------------------------------------------------------
  // Message Classes
  //-----------------------------------------------------------------------------------

  MESSAGE_CLASS_HEADER(None) };
  MESSAGE_CLASS_HEADER(GoBack) };

  MESSAGE_CLASS_HEADER(GUI_MouseMove)
    int32_t x;
    int32_t y;
  };

  MESSAGE_CLASS_HEADER(GUI_MouseButtonUp)
    uint32_t  button;
    int32_t   x;
    int32_t   y;
  };

  MESSAGE_CLASS_HEADER(GUI_MouseButtonDown)
    uint32_t  button;
    int32_t   x;
    int32_t   y;
  };

  MESSAGE_CLASS_HEADER(GUI_KeyUp)
    uint32_t  keyCode;
    uint16_t  modState;
  };

  MESSAGE_CLASS_HEADER(GUI_KeyDown)
    uint32_t  keyCode;
    uint16_t  modState;
  };

  MESSAGE_CLASS_HEADER(GUI_MouseWheelUp) };
  MESSAGE_CLASS_HEADER(GUI_MouseWheelDown) };

  MESSAGE_CLASS_HEADER(GUI_Text)
    char text[TEXT_INPUT_TEXT_SIZE];
  };

  MESSAGE_CLASS_HEADER(Window_Shown) };
  MESSAGE_CLASS_HEADER(Window_Hidden) };
  MESSAGE_CLASS_HEADER(Window_Exposed) };

  MESSAGE_CLASS_HEADER(Window_Moved)
    int32_t   x;
    int32_t   y;
  };

  MESSAGE_CLASS_HEADER(Window_Resized)
    int32_t   w;
    int32_t   h;
  };

  MESSAGE_CLASS_HEADER(Window_Size_Changed)
    int32_t   w;
    int32_t   h;
  };

  MESSAGE_CLASS_HEADER(Window_Minimized) };
  MESSAGE_CLASS_HEADER(Window_Maximized) };
  MESSAGE_CLASS_HEADER(Window_Restored) };
  MESSAGE_CLASS_HEADER(Window_Enter) };
  MESSAGE_CLASS_HEADER(Window_Leave) };
  MESSAGE_CLASS_HEADER(Window_Focus_Gained) };
  MESSAGE_CLASS_HEADER(Window_Focus_Lost) };
  MESSAGE_CLASS_HEADER(Window_Close) };
  MESSAGE_CLASS_HEADER(Window_Take_Focus) };

  MESSAGE_CLASS_HEADER(Input_Text)
    char text[TEXT_INPUT_TEXT_SIZE];
  };

  MESSAGE_CLASS_HEADER(Input_KeyUp)
    uint32_t  keyCode;
    uint16_t   modState;
  };

  MESSAGE_CLASS_HEADER(Input_KeyDown)
    uint32_t  keyCode;
    uint16_t   modState;
  };

  MESSAGE_CLASS_HEADER(Input_MouseButtonUp)
    uint32_t  button;
    int32_t    x;
    int32_t    y;
  };

  MESSAGE_CLASS_HEADER(Input_MouseButtonDown)
    uint32_t  button;
    int32_t    x;
    int32_t    y;
  };

  MESSAGE_CLASS_HEADER(Input_MouseWheelUp) };
  MESSAGE_CLASS_HEADER(Input_MouseWheelDown) };

  MESSAGE_CLASS_HEADER(Input_MouseMove)
    int32_t x;
    int32_t y;
  };

  MESSAGE_CLASS_HEADER(Command)
    template<typename FuncT>
    static TRef<Message> New(FuncT&& func)
    {
      static_assert(std::is_trivially_destructible<FuncT>::value, "FuncT must be trivially destructible");
      MessageCommandFn renderCmd = [](void* ptr)
      {
        auto pFunc = (FuncT*)ptr;
        (*pFunc)();
        pFunc->~FuncT();
      };

      size_t sze = sizeof(uint64_t) + sizeof(MessageCommandFn) + sizeof(func);
      void* pBuf = TBUFAlloc(sze);

      *static_cast<uint64_t*>(pBuf) = sze;

      void* ptr = AdvancePtr(pBuf, sizeof(uint64_t));
      *static_cast<MessageCommandFn*>(ptr) = renderCmd;

      ptr = AdvancePtr(ptr, sizeof(MessageCommandFn));
      new (ptr) FuncT(std::forward<FuncT>(func));

      TRef<Message_Command> msg = TRef<Message_Command>::New(pBuf);
      return DynamicPointerCast<Message>(msg);
    }

    Message_Command();
    Message_Command(void* a_ptr);

    void Run();

    void* ptr;
  };

  //-----------------------------------------------------------------------------------
  // Make sure messages are trivially destructable
  //-----------------------------------------------------------------------------------
#undef ITEM
#define ITEM(MESSAGE_TYPE) static_assert(std::is_trivially_destructible<Message_##MESSAGE_TYPE>::value, #MESSAGE_TYPE " must be trivially destructible");\
  MESSAGE_LIST

  //-----------------------------------------------------------------------------------
  // Message translators
  //-----------------------------------------------------------------------------------
  
  namespace MessageTranslator
  {
    typedef void (*MessageTranslatorFn)(Message * dest, Message const * src);

    void Clear();
    bool Exists(uint32_t, uint32_t);
    void AddTranslator(uint32_t, uint32_t, MessageTranslatorFn); //Will override
    void Translate(Message * dest, Message const * src);
    void AddDefaultTranslators();
  }
}
#endif