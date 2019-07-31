#include "Layer_Window.h"
#include "Message.h"
#include "Game.h"

Layer_Window::Layer_Window(MessageBus * a_pMsgBus, IWindow * a_pWindow)
  : Layer(a_pMsgBus)
  , m_pWindow(a_pWindow)
{

}

Layer_Window::~Layer_Window()
{

}

void Layer_Window::OnAttach()
{

}

void Layer_Window::OnDetach()
{

}

void Layer_Window::Update()
{

}

bool Layer_Window::HandleMessage(Message const & a_msg)
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