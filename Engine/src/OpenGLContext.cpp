//@group Renderer

#include "OpenGLContext.h"
#include "Framework.h"
#include "glad/glad.h"
#include "core_Log.h"
#include "core_ErrorCodes.h"
#include "core_Assert.h"
#include "SDL.h"

namespace Engine
{
  void Framework::InitGraphicsContext()
  {
    SetGraphicsContext(new OpenGLContext());
  }

  OpenGLContext::~OpenGLContext()
  {
    SDL_GL_DeleteContext(m_context);
    m_context = nullptr;
  }

  OpenGLContext::OpenGLContext()
    : m_pWindow(nullptr)
    , m_context(nullptr)
  {

  }

  Core::ErrorCode OpenGLContext::Init()
  {
    return Core::EC_None;
  }

  Core::ErrorCode OpenGLContext::Init(SDL_Window* a_pWindow)
  {
    BSR_ASSERT(a_pWindow != nullptr);
    m_pWindow = a_pWindow;

    // Create OpenGL context
    m_context = SDL_GL_CreateContext(m_pWindow);
    if (m_context == nullptr)
    {
      LOG_ERROR("SDL_GL_CreateContext() Failed!");
      return Core::EC_Error;
    }
    LOG_TRACE("Opengl loaded");

    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
    {
      LOG_ERROR("Glad failed to log");
      return Core::EC_Error;
    }

    LOG_TRACE("Vendor:   {}", glGetString(GL_VENDOR));
    LOG_TRACE("Renderer: {}", glGetString(GL_RENDERER));
    LOG_TRACE("Version:  {}", glGetString(GL_VERSION));

    return Core::EC_None;
  }

  void OpenGLContext::SwapBuffers()
  {
    SDL_GL_SwapWindow(m_pWindow);

    //TODO move this to the renderer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

}
