#include "Layer_Application.h"
#include "Message.h"

namespace Engine
{
  Layer_Application::Layer_Application()
  {
  
  }

  Layer_Application::~Layer_Application()
  {

  }

  void Layer_Application::OnAttach()
  {

  }

  void Layer_Application::OnDetach()
  {

  }

  void Layer_Application::Update(float a_dt)
  {

  }

  void Layer_Application::HandleMessage(MessageSub<MT_Command> * a_pMsg)
  {
    void* ptr = a_pMsg->ptr;

    ptr = AdvancePtr(ptr, sizeof(uint64_t));

    MessageCommandFn function = *(MessageCommandFn*)ptr;
    ptr = AdvancePtr(ptr, sizeof(MessageCommandFn));

    function(ptr);
    a_pMsg->SetFlag(Message::Flag::Handled);
  }
}