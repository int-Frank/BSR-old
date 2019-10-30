#ifndef EN_FRAMEWORK_H
#define EN_FRAMEWORK_H

#include <memory>
#include <string>

#include "core_ErrorCodes.h"

#include "../Engine/IWindow.h"
#include "../Engine/IEventPoller.h"
#include "../Engine/IMouseController.h"
#include "../Engine/IShader.h"
#include "../Engine/Buffer.h"

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
    static Core::ErrorCode Init();
    static Core::ErrorCode ShutDown();

    //There can only be one of these objects...
    std::shared_ptr<IWindow>          GetWindow();
    std::shared_ptr<IEventPoller>     GetEventPoller();
    std::shared_ptr<IMouseController> GetMouseController();

    //Rendering
    IShader * CreateShader(std::string const & vs, std::string const & fs);
    IVertexBuffer * CreateVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Static);
    IVertexBuffer * CreateVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
    IIndexBuffer * CreateIndexBuffer(void* data, uint32_t size = 0);

    //Audio
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