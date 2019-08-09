#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <memory>
#include <string>

#include "../Engine/ErrorCodes.h"

#include "../Engine/IWindow.h"
#include "../Engine/IEventPoller.h"
#include "../Engine/IMouseController.h"
#include "../Engine/IShader.h"

namespace Engine
{
  class Framework
  {
  public:

    struct ImGui_InitData
    {
      int window_w;
      int window_h;
    };

  public:

    Framework();
    ~Framework();

    static Framework * Instance();
    static ErrorCode Init();
    static ErrorCode ShutDown();

    //These will pass pointers to internal objects.
    //Do NOT delete these! All internal objects will
    //Destroyed on calling DestroyInstance();

    //There can only be one of these objects...
    std::shared_ptr<IWindow>          GetWindow();
    std::shared_ptr<IEventPoller>     GetEventPoller();
    std::shared_ptr<IMouseController> GetMouseController();

    IShader * GetShader(std::string const & vs, std::string const & fs);

    //IAudio *       GetAudio();

    //These can only be initialized after the window has been created.
    bool InitImGui(ImGui_InitData const &);

  private:

    //These are implemented in the relevent cpp files
    void InitWindow();
    void InitEventPoller();
    void InitMouseController();

  private:

    void SetWindow(IWindow *);
    void SetEventPoller(IEventPoller *);
    void SetMouseController(IMouseController *);

  private:

    static Framework * s_instance;

    class PIMPL;
    PIMPL * m_pimpl;

  };
}


#endif