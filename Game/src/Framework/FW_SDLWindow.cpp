#include "SDL.h"
#include "glad/glad.h"

#include "Framework.h"
#include "../Options.h"
#include "../Log.h"
#include "../IWindow.h"

#define OPENGL_MAJOR 4
#define OPENGL_MINOR 6

class FW_SDLWindow : public IWindow
{
public:

  FW_SDLWindow();
  ~FW_SDLWindow();

  void Update();

  void SetVSync(bool);
  bool IsVSync() const;

  bool IsInit() const;
  ErrorCode Init(WindowProps const & props = WindowProps());
  void Destroy();

  void GetDimensions(int & w, int & h);

private:

  SDL_Window *  m_window;
  SDL_GLContext m_glContext;
};

void Framework::InitWindow()
{
  SetWindow(new FW_SDLWindow());
}

FW_SDLWindow::FW_SDLWindow()
  : m_window(nullptr)
  , m_glContext(nullptr)
{

}

FW_SDLWindow::~FW_SDLWindow()
{
  Destroy();
}

void FW_SDLWindow::Update()
{
  SDL_GL_SwapWindow(m_window);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  return m_window != nullptr;
}

ErrorCode FW_SDLWindow::Init(WindowProps const & a_props)
{
  BSR_ASSERT(m_window == nullptr && m_glContext == nullptr, "FW_SDLWindow already initialised!");

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // Create window
  m_window = SDL_CreateWindow(a_props.name.c_str(), 100, 100,
                              a_props.width, a_props.height, SDL_WINDOW_OPENGL);
  if(m_window == NULL)
  {
    LOG_ERROR("Failed to create window!");
    return EC_Error;
  }

  // Create OpenGL context
  m_glContext = SDL_GL_CreateContext(m_window);
  if(m_glContext == NULL)
  {
    LOG_ERROR("Failed to create opengl context!");
    return EC_Error;
  }

  LOG_TRACE("Opengl loaded");
  if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
  {
    LOG_ERROR("Glad failed to log");
    return EC_Error;
  }

  LOG_TRACE("Vendor:   {}", glGetString(GL_VENDOR));
  LOG_TRACE("Renderer: {}", glGetString(GL_RENDERER));
  LOG_TRACE("Version:  {}", glGetString(GL_VERSION));

  return EC_None;
}

void FW_SDLWindow::Destroy()
{
  SDL_GL_DeleteContext(m_glContext);
  SDL_DestroyWindow(m_window);
  m_window = nullptr;
  m_glContext = nullptr;
}

void FW_SDLWindow::GetDimensions(int & a_w, int & a_h)
{
  SDL_GetWindowSize(m_window, &a_w, &a_h);
}