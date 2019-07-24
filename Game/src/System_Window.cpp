#include "System_Window.h"
#include "Message.h"
#include "Game.h"

System_Window::System_Window(MessageBus * a_pMsgBus, IWindow * a_pWindow)
  : System(a_pMsgBus)
  , m_pWindow(a_pWindow)
{

}

System_Window::~System_Window()
{

}

void System_Window::OnAttach()
{

}

void System_Window::OnDetach()
{

}

void System_Window::Update()
{

}

bool System_Window::HandleMessage(Message const & a_msg)
{
  if (!(a_msg.type & MC_Window))
    return false;

  bool result = true;
  switch (a_msg.type)
  {
    case MT_Window_Close:
    {
      Game::Instance()->RequestQuit();
      break;
    }
    default:
    {
      result = false;
      break;
    }
  }

  return result;
}