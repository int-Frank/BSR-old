//@group Layers

#ifndef EN_LAYER_WINDOW_H
#define EN_LAYER_WINDOW_H

#include "Memory.h"

#include "core_ErrorCodes.h"
#include "Layer.h"
#include "Memory.h"

namespace Engine
{
  class IWindow;

  class Layer_Window : public Layer
  {
  public:

    static uint32_t GetID()
    {
      return 5;
    }

    uint32_t GetThisID() override
    {
      return 5;
    }

    Layer_Window(Ref<IWindow>);
    ~Layer_Window();

    void OnAttach();
    void OnDetach();

    void Update(float);

    void HandleMessage(MessageSub<MT_Window_Shown> *) override;
    void HandleMessage(MessageSub<MT_Window_Hidden> *) override;
    void HandleMessage(MessageSub<MT_Window_Exposed> *) override;
    void HandleMessage(MessageSub<MT_Window_Moved> *) override;
    void HandleMessage(MessageSub<MT_Window_Resized> *) override;
    void HandleMessage(MessageSub<MT_Window_Size_Changed> *) override;
    void HandleMessage(MessageSub<MT_Window_Minimized> *) override;
    void HandleMessage(MessageSub<MT_Window_Maximized> *) override;
    void HandleMessage(MessageSub<MT_Window_Restored> *) override;
    void HandleMessage(MessageSub<MT_Window_Enter> *) override;
    void HandleMessage(MessageSub<MT_Window_Leave> *) override;
    void HandleMessage(MessageSub<MT_Window_Focus_Gained> *) override;
    void HandleMessage(MessageSub<MT_Window_Focus_Lost> *) override;
    void HandleMessage(MessageSub<MT_Window_Close> *) override;
    void HandleMessage(MessageSub<MT_Window_Take_Focus> *) override;

  private:

    Ref<IWindow> m_pWindow;
  };
}


#endif