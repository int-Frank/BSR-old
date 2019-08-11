#ifndef LAYER_WINDOW_H
#define LAYER_WINDOW_H

#include <memory>

#include "ErrorCodes.h"
#include "Layer.h"

namespace Engine
{
  class IWindow;

  class Layer_Window : public Layer
  {
  public:

    ASSIGN_ID

      Layer_Window(MessageBus * a_pMsgBus, std::shared_ptr<IWindow>);
    ~Layer_Window();

    void OnAttach();
    void OnDetach();

    void Update(float);

    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Shown>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Hidden>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Exposed>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Moved>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Resized>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Size_Changed>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Minimized>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Maximized>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Restored>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Enter>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Leave>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Focus_Gained>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Focus_Lost>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Close>*);
    MessageHandlerReturnCode HandleMessage(MessageSub<MT_Window_Take_Focus>*);

  private:

    std::shared_ptr<IWindow> m_pWindow;
  };
}


#endif