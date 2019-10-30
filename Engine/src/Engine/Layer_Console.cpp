#include "Layer_Console.h"
#include "core_Log.h"

#define LOG_MESSAGE(...) LOG_TRACE(__VA_ARGS__)

namespace Engine
{
  Layer_Console::Layer_Console(MessageBus * a_msgBus)
    : Layer(a_msgBus)
  {

  }

  Layer_Console::~Layer_Console()
  {

  }

  void Layer_Console::Update(float a_dt)
  {

  }

#undef ITEM
#define ITEM(x) void Layer_Console::HandleMessage(MessageSub<MT_##x> * a_pMsg)\
{if (a_pMsg->flags & Message::E_Show) LOG_TRACE(a_pMsg->ToString());}
  MESSAGE_LIST

}
