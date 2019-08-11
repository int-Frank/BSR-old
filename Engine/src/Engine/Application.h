#ifndef GAME_H
#define GAME_H

#include "MessageBus.h"
#include "LayerStack.h"
#include "../Framework/Framework.h"

namespace Engine
{

  class Layer;
  class IWindow;

  class Application
  {
  public:

    Application();
    virtual ~Application();

    static Application * Instance();

    void Run();
    void RequestQuit();

  private:

    Application(Application const &) = delete;
    Application & operator=(Application const &) = delete;

    void InitWindow();

  private:

    static Application * s_instance;

  private:

    bool m_shouldQuit;
    std::shared_ptr<IWindow> m_pWindow;

    LayerStack  m_layerStack;
    MessageBus  m_msgBus;
  };

  // To be defined in CLIENT
  Application * CreateApplication();
}

#endif