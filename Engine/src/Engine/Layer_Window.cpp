
#include "Memory.h"

#include "Layer_Window.h"
#include "Message.h"
#include "Application.h"
#include "IWindow.h"

namespace Engine
{
  Layer_Window::Layer_Window(MessageBus * a_pMsgBus, Ref<IWindow> a_pWindow)
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

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Shown> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Hidden> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Exposed> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Moved> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Resized> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Size_Changed> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Minimized> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Maximized> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Restored> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Enter> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Leave> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Focus_Gained> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Focus_Lost> *)
  {
    
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Close> * a_pMsg)
  {
    Application::Instance()->RequestQuit();
    a_pMsg->SetFlag(Message::Flag::Handled);
  }

  void Layer_Window::HandleMessage(MessageSub<MT_Window_Take_Focus> *)
  {

  }
}
