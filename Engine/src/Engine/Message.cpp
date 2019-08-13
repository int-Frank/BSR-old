#include "Message.h"
#include "MessageHandler.h"
#include "DgMap_AVL.h"

#undef ITEM
#define ITEM(x) void MessageSub<MT_##x>::Submit(MessageHandler * a_pHandler)\
    {a_pHandler->HandleMessage(static_cast<MessageSub<MT_##x> *>(this));}\

namespace Engine
{
  MESSAGE_TYPES

  namespace MessageTranslator
  {
    static Dg::Map_AVL<uint64_t, MessageTranslatorFn> g_msgTrnsMap;

    static void Translate_GUI_MouseMove__Input_MouseMove(Message * a_pDest, Message const * a_pSrc)
    {
      MessageSub<MT_GUI_MouseMove> * pDest 
        = static_cast<MessageSub<MT_GUI_MouseMove> *> (a_pDest);
      MessageSub<MT_Input_MouseMove> const* pSrc 
        = static_cast<MessageSub<MT_Input_MouseMove> const *> (a_pSrc);

      pDest->x = pSrc->x;
      pDest->y = pSrc->y;
    }

    static void Translate_GUI_Text__Input_Text(Message * a_pDest, Message const * a_pSrc)
    {
      MessageSub<MT_GUI_Text> * pDest 
        = static_cast<MessageSub<MT_GUI_Text> *> (a_pDest);
      MessageSub<MT_Input_Text> const* pSrc 
        = static_cast<MessageSub<MT_Input_Text> const *> (a_pSrc);

      strncpy_s(pDest->text, TEXT_INPUT_TEXT_SIZE, pSrc->text, TEXT_INPUT_TEXT_SIZE);
    }

    static void Translate_GUI_KeyDown__Input_KeyDown(Message * a_pDest, Message const * a_pSrc)
    {
      MessageSub<MT_GUI_KeyDown> * pDest 
        = static_cast<MessageSub<MT_GUI_KeyDown> *> (a_pDest);
      MessageSub<MT_Input_KeyDown> const* pSrc 
        = static_cast<MessageSub<MT_Input_KeyDown> const *> (a_pSrc);

      pDest->keyCode = pSrc->keyCode;
      pDest->modState = pSrc->modState;
    }

    static void Translate_GUI_KeyUp__Input_KeyUp(Message * a_pDest, Message const * a_pSrc)
    {
      MessageSub<MT_GUI_KeyUp> * pDest 
        = static_cast<MessageSub<MT_GUI_KeyUp> *> (a_pDest);
      MessageSub<MT_Input_KeyUp> const* pSrc 
        = static_cast<MessageSub<MT_Input_KeyUp> const *> (a_pSrc);

      pDest->keyCode = pSrc->keyCode;
      pDest->modState = pSrc->modState;
    }

    static void Translate_GUI_MouseButtonDown__Input_MouseButtonDown(Message * a_pDest, Message const * a_pSrc)
    {
      MessageSub<MT_GUI_MouseButtonDown> * pDest 
        = static_cast<MessageSub<MT_GUI_MouseButtonDown> *> (a_pDest);
      MessageSub<MT_Input_MouseButtonDown> const* pSrc 
        = static_cast<MessageSub<MT_Input_MouseButtonDown> const *> (a_pSrc);

      pDest->button = pSrc->button;
      pDest->x = pSrc->x;
      pDest->y = pSrc->y;
    }

    static void Translate_GUI_MouseButtonUp__Input_MouseButtonUp(Message * a_pDest, Message const * a_pSrc)
    {
      MessageSub<MT_GUI_MouseButtonUp> * pDest 
        = static_cast<MessageSub<MT_GUI_MouseButtonUp> *> (a_pDest);
      MessageSub<MT_Input_MouseButtonUp> const* pSrc 
        = static_cast<MessageSub<MT_Input_MouseButtonUp> const *> (a_pSrc);

      pDest->button = pSrc->button;
      pDest->x = pSrc->x;
      pDest->y = pSrc->y;
    }

    static uint64_t PackKey(uint32_t a_dest, uint32_t a_src)
    {
      return (uint64_t(a_dest) << 32) | a_src;
    }

    void Clear()
    {
      g_msgTrnsMap.clear();
    }

    bool Exists(MessageType a_dest, MessageType a_src)
    {
      uint64_t key = PackKey(a_dest, a_src);
      return g_msgTrnsMap.find(key) != g_msgTrnsMap.end();
    }

    void Translate(Message * a_dest, Message const * a_src)
    {
      uint64_t key = PackKey(a_dest->GetID(), a_src->GetID());
      auto it = g_msgTrnsMap.find(key);
      if (it != g_msgTrnsMap.end())
        it->second(a_dest, a_src);
    }

    void AddTranslator(MessageType a_dest, MessageType a_src,
                       MessageTranslatorFn a_fn)
    {
      uint64_t key = PackKey(a_dest, a_src);
      g_msgTrnsMap[key] = a_fn;
    }

    void AddDefaultTranslators()
    {
      AddTranslator(MT_GUI_MouseMove, MT_Input_MouseMove, Translate_GUI_MouseMove__Input_MouseMove);
      AddTranslator(MT_GUI_Text, MT_Input_Text, Translate_GUI_Text__Input_Text);
      AddTranslator(MT_GUI_KeyDown, MT_Input_KeyDown, Translate_GUI_KeyDown__Input_KeyDown);
      AddTranslator(MT_GUI_KeyUp, MT_Input_KeyUp, Translate_GUI_KeyUp__Input_KeyUp);
      AddTranslator(MT_GUI_MouseButtonDown, MT_Input_MouseButtonDown, Translate_GUI_MouseButtonDown__Input_MouseButtonDown);
      AddTranslator(MT_GUI_MouseButtonUp, MT_Input_MouseButtonUp, Translate_GUI_MouseButtonUp__Input_MouseButtonUp);
    }
  }
}