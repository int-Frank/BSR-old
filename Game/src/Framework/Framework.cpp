
#include "SDL.h"
#include "../Options.h"
#include "../Log.h"
#include "Framework.h"
#include "FW_SDLWindow.h"

class Framework::PIMPL
{
public:

  PIMPL()
    : window(nullptr)
  {
  
  }

  ~PIMPL()
  {
    delete window;
  }

  IWindow * window;
};

Framework * Framework::s_instance(nullptr);

Framework::Framework()
  : m_pimpl(new PIMPL())
{

}

Framework::~Framework()
{
  delete m_pimpl;
}

Framework * Framework::Instance()
{
  if (s_instance == nullptr)
    s_instance = new Framework();
  return s_instance;
}

void Framework::DestroyInstance()
{
  delete s_instance;
  s_instance = nullptr;
}

ErrorCode Framework::Init()
{
  ErrorCode result = EC_None;
  do
  {
    //-----------------------------------------------------------------------------------------
    //Init APIs...
    //-----------------------------------------------------------------------------------------

    //Init SDL...
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
      LOG_ERROR("Unable to initialize SDL: {}", SDL_GetError());
      result = EC_Error;
      break;
    }

    //Init Glad
    /*if (gladLoadGLLoader(SDL_GL_GetProcAddress) ==0)
    {
      LOG_ERROR("Unable to initialize GLAD");
      result = EC_Error;
      break;
    }*/

    //-----------------------------------------------------------------------------------------
    //Init Modules...
    //-----------------------------------------------------------------------------------------

  } while (false);
  return result;
}

ErrorCode Framework::ShutDown()
{
  ErrorCode result = EC_None;

  SDL_Quit();

  return result;
}

IWindow * Framework::GetWindow()
{
  if (m_pimpl->window == nullptr)
    m_pimpl->window = new FW_SDLWindow();
  return m_pimpl->window;
}