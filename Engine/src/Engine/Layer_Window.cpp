
#include <memory>

#include "Layer_Window.h"
#include "Message.h"
#include "Application.h"

namespace Engine
{
  Layer_Window::Layer_Window(MessageBus * a_pMsgBus, std::shared_ptr<IWindow> a_pWindow)
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

  void Layer_Window::Update(float a_dt)
  {
    m_pWindow->Update();
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Shown>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Hidden>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Exposed>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Moved>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Resized>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Size_Changed>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Minimized>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Maximized>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Restored>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Enter>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Leave>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Focus_Gained>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Focus_Lost>*)
  {
    return MessageHandlerReturnCode::None;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Close>*)
  {
    Application::Instance()->RequestQuit();
    return MessageHandlerReturnCode::Consumed;
  }

  MessageHandlerReturnCode Layer_Window::HandleMessage(MessageSub<MT_Window_Take_Focus>*)
  {
    return MessageHandlerReturnCode::None;
  }
}
