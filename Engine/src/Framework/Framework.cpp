
#include "SDL.h"
#include "../Engine/Options.h"
#include "../Engine/Log.h"
#include "Framework.h"
#include "../Engine/InputCodes.h"
#include "../Engine/BSR_Assert.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"

namespace Engine
{
  static char*        g_ClipboardTextData = nullptr;

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

    }

    std::shared_ptr<IWindow>          window;
    std::shared_ptr<IEventPoller>     eventPoller;
    std::shared_ptr<IMouseController> mouseController;
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

    if (g_ClipboardTextData)
      SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = nullptr;

    SDL_Quit();

    return result;
  }

  std::shared_ptr<IWindow> Framework::GetWindow()
  {
    return m_pimpl->window;
  }

  std::shared_ptr<IEventPoller> Framework::GetEventPoller()
  {
    return m_pimpl->eventPoller;
  }

  std::shared_ptr<IMouseController> Framework::GetMouseController()
  {
    return m_pimpl->mouseController;
  }

  void Framework::SetWindow(IWindow * a_window)
  {
    BSR_ASSERT(m_pimpl->window == nullptr, "Window already exists!");
    m_pimpl->window = std::shared_ptr<IWindow>(a_window);
  }

  void Framework::SetEventPoller(IEventPoller * a_ep)
  {
    BSR_ASSERT(m_pimpl->eventPoller == nullptr, "EventPoller already exists!");
    m_pimpl->eventPoller = std::shared_ptr<IEventPoller>(a_ep);
  }

  void Framework::SetMouseController(IMouseController * a_mc)
  {
    BSR_ASSERT(m_pimpl->mouseController == nullptr, "MouseController already exists!");
    m_pimpl->mouseController = std::shared_ptr<IMouseController>(a_mc);
  }

  static const char* ImGui_ImplSDL2_GetClipboardText(void*)
  {
    if (g_ClipboardTextData)
      SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = SDL_GetClipboardText();
    return g_ClipboardTextData;
  }

  static void ImGui_ImplSDL2_SetClipboardText(void*, const char* text)
  {
    SDL_SetClipboardText(text);
  }

  bool Framework::InitImGui(ImGui_InitData const & a_data)
  {
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init("#version 410");

    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2(float(a_data.window_w), float(a_data.window_h));
    io.DisplayFramebufferScale = ImVec2(1.f, 1.f);

    io.KeyMap[ImGuiKey_Tab] = IC_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = IC_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = IC_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = IC_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = IC_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = IC_KEY_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = IC_KEY_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = IC_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = IC_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = IC_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = IC_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = IC_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = IC_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = IC_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = IC_KEY_ESC;
    io.KeyMap[ImGuiKey_KeyPadEnter] = IC_KEY_KPENTER;
    io.KeyMap[ImGuiKey_A] = IC_KEY_A;
    io.KeyMap[ImGuiKey_C] = IC_KEY_C;
    io.KeyMap[ImGuiKey_V] = IC_KEY_V;
    io.KeyMap[ImGuiKey_X] = IC_KEY_X;
    io.KeyMap[ImGuiKey_Y] = IC_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = IC_KEY_Z;

    io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
    io.ClipboardUserData = nullptr;

    return true;
  }
}
