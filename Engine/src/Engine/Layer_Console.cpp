#include "Layer_Console.h"
#include "Log.h"

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
#define ITEM(x) MessageHandlerReturnCode Layer_Console::HandleMessage(MessageSub<MT_##x> * a_pMsg)\
{\
  LOG_TRACE(a_pMsg->ToString());\
  return MessageHandlerReturnCode::None;\
}
  MESSAGE_TYPES

}
