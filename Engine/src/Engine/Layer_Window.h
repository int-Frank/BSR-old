#ifndef EN_LAYER_WINDOW_H
#define EN_LAYER_WINDOW_H

#include <memory>

#include "core_ErrorCodes.h"
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

    void HandleMessage(MessageSub<MT_Window_Shown>*);
    void HandleMessage(MessageSub<MT_Window_Hidden>*);
    void HandleMessage(MessageSub<MT_Window_Exposed>*);
    void HandleMessage(MessageSub<MT_Window_Moved>*);
    void HandleMessage(MessageSub<MT_Window_Resized>*);
    void HandleMessage(MessageSub<MT_Window_Size_Changed>*);
    void HandleMessage(MessageSub<MT_Window_Minimized>*);
    void HandleMessage(MessageSub<MT_Window_Maximized>*);
    void HandleMessage(MessageSub<MT_Window_Restored>*);
    void HandleMessage(MessageSub<MT_Window_Enter>*);
    void HandleMessage(MessageSub<MT_Window_Leave>*);
    void HandleMessage(MessageSub<MT_Window_Focus_Gained>*);
    void HandleMessage(MessageSub<MT_Window_Focus_Lost>*);
    void HandleMessage(MessageSub<MT_Window_Close>*);
    void HandleMessage(MessageSub<MT_Window_Take_Focus>*);

  private:

    std::shared_ptr<IWindow> m_pWindow;
  };
}


#endif