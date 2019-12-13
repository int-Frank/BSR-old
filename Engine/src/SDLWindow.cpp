//@group Framework


#include "SDL.h"
#include "glad/glad.h"

#include "Framework.h"
#include "Options.h"
#include "core_Log.h"
#include "IWindow.h"
#include "core_Assert.h"
#include "OpenGLContext.h"

#define OPENGL_MAJOR 4
#define OPENGL_MINOR 6

namespace Engine
{
  class FW_SDLWindow : public IWindow
  {
  public:

    FW_SDLWindow();
    ~FW_SDLWindow();

    void Update();

    void SetVSync(bool);
    bool IsVSync() const;

    bool IsInit() const;
    Core::ErrorCode Init(WindowProps const & props = WindowProps());
    void Destroy();

    void GetDimensions(int & w, int & h);

  private:

    SDL_Window *        m_pWindow;
    IGraphicsContext *  m_pContext; //TODO this context should live in Framework
  };

  void Framework::InitWindow()
  {
    SetWindow(new FW_SDLWindow());
  }

  FW_SDLWindow::FW_SDLWindow()
    : m_pWindow(nullptr)
    , m_pContext(nullptr)
  {

  }

  FW_SDLWindow::~FW_SDLWindow()
  {
    Destroy();
  }

  void FW_SDLWindow::Update()
  {
    m_pContext->SwapBuffers();
  }

  void FW_SDLWindow::SetVSync(bool a_val)
  {
    if (a_val)
      SDL_GL_SetSwapInterval(1);
    else
      SDL_GL_SetSwapInterval(0);
  }

  bool FW_SDLWindow::IsVSync() const
  {
    return SDL_GL_GetSwapInterval() == 1;
  }

  bool FW_SDLWindow::IsInit() const
  {
    return m_pWindow != nullptr;
  }

  Core::ErrorCode FW_SDLWindow::Init(WindowProps const & a_props)
  {
    BSR_ASSERT(m_pWindow == nullptr && m_pContext == nullptr, "FW_SDLWindow already initialised!");

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    m_pWindow = SDL_CreateWindow(a_props.name.c_str(), 100, 100,
      a_props.width, a_props.height, SDL_WINDOW_OPENGL);
    if(m_pWindow == nullptr)
    {
      LOG_ERROR("Failed to create window!");
      return Core::EC_Error;
    }

    m_pContext = new OpenGLContext(m_pWindow);
    if (m_pContext->Init() != Core::EC_None)
    {
      LOG_ERROR("Failed to create opengl context!");
      Destroy();
      return Core::EC_Error;
    }

    return Core::EC_None;
  }

  void FW_SDLWindow::Destroy()
  {
    delete m_pContext;
    m_pContext = nullptr;

    SDL_DestroyWindow(m_pWindow);
    m_pWindow = nullptr;
  }

  void FW_SDLWindow::GetDimensions(int & a_w, int & a_h)
  {
    SDL_GetWindowSize(m_pWindow, &a_w, &a_h);
  }
}
