
#include "SDL.h"
#include "../Options.h"
#include "../Log.h"
#include "Framework.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"

class Framework::PIMPL
{
public:

  PIMPL()
    : window(nullptr)
    , eventPoller(nullptr)
    , mouseController(nullptr)
  {
  
  }

  ~PIMPL()
  {
    delete window;
    delete eventPoller;
    delete mouseController;

    window = nullptr;
    eventPoller = nullptr;
    mouseController = nullptr;
  }

  IWindow *       window;
  IEventPoller *  eventPoller;
  IMouseController * mouseController;
};

Framework * Framework::s_instance(nullptr);

Framework::Framework()
  : m_pimpl(new PIMPL())
{

}

Framework::~Framework()
{
  delete m_pimpl;

  ImGui_ImplOpenGL3_Shutdown();
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
    s_instance->InitWindow(); //Init window, create OpenGL context, init GLAD
    s_instance->InitEventPoller();
    s_instance->InitMouseController();

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

IMouseController * Framework::GetMouseController()
{
  return m_pimpl->mouseController;
}

void Framework::SetWindow(IWindow * a_window)
{
  BSR_ASSERT(m_pimpl->window == nullptr, "Window already exists!");
  m_pimpl->window = a_window;
}

void Framework::SetEventPoller(IEventPoller * a_ep)
{
  BSR_ASSERT(m_pimpl->eventPoller == nullptr, "EventPoller already exists!");
  m_pimpl->eventPoller = a_ep;
}

void Framework::SetMouseController(IMouseController * a_mc)
{
  BSR_ASSERT(m_pimpl->mouseController == nullptr, "MouseController already exists!");
  m_pimpl->mouseController = a_mc;
}

bool Framework::InitImGui()
{
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 410");

  ImGuiIO& io = ImGui::GetIO();

  io.DisplaySize = ImVec2(1024.f, 768.f);
  io.DisplayFramebufferScale = ImVec2(1.f, 1.f);

  return true;
}