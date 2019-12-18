//@group Layers

#ifndef EN_LAYER_APPLICATION_H
#define EN_LAYER_APPLICATION_H

#include "Memory.h"

#include "core_ErrorCodes.h"
#include "Layer.h"
#include "Memory.h"

namespace Engine
{
  class Layer_Application : public Layer
  {
  public:

    static uint32_t GetID()
    {
      return 1;
    }

    uint32_t GetThisID() override
    {
      return 1;
    }

      Layer_Application();
    ~Layer_Application();

    void OnAttach();
    void OnDetach();

    void Update(float);

    void HandleMessage(MessageSub<MT_Command>*) override;

  private:

  };
}


#endif