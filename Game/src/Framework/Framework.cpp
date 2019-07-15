
#include "SDL.h"
#include "../Options.h"
#include "../Log.h"
#include "Framework.h"

class Framework::PIMPL
{
public:

  PIMPL()
    : window(nullptr)
    , eventPoller(nullptr)
  {
  
  }

  ~PIMPL()
  {
    delete window;
    delete eventPoller;

    window = nullptr;
    eventPoller = nullptr;
  }

  IWindow *       window;
  IEventPoller *  eventPoller;
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
  BSR_ASSERT(s_instance != nullptr, "Framework not initialized!");
  return s_instance;
}

ErrorCode Framework::Init()
{
  BSR_ASSERT(s_instance == nullptr, "Framework already initialized!");
  s_instance = new Framework();
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

    //-----------------------------------------------------------------------------------------
    //Init Modules...
    //-----------------------------------------------------------------------------------------
    s_instance->InitWindow();
    s_instance->InitEventPoller();

  } while (false);
  return result;
}

ErrorCode Framework::ShutDown()
{
  ErrorCode result = EC_None;

  delete s_instance;
  s_instance = nullptr;

  SDL_Quit();

  return result;
}

IWindow * Framework::GetWindow()
{
  return m_pimpl->window;
}

IEventPoller * Framework::GetEventPoller()
{
  return m_pimpl->eventPoller;
}

void Framework::SetWindow(IWindow * a_window)
{
  m_pimpl->window = a_window;
}

void Framework::SetEventPoller(IEventPoller * a_ep)
{
  m_pimpl->eventPoller = a_ep;
}