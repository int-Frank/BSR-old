#include <sstream>

#include "Framework.h"
#include "../Message.h"
#include "../InputCodes.h"
#include "../WindowCodes.h"
#include "../Log.h"

#include "SDL_events.h"
#include "SDL.h"

#include "../IEventPoller.h"

class FW_EventPoller : public IEventPoller
{
public:

  FW_EventPoller();
  ~FW_EventPoller();
  bool NextEvent(Message &);
};

void Framework::InitEventPoller()
{
  SetEventPoller(new FW_EventPoller());
}

FW_EventPoller::FW_EventPoller()
{

}

FW_EventPoller::~FW_EventPoller()
{

}

static uint32_t TranslateKeyCode(SDL_Keycode a_key)
{
  //SDL keys directly map onto KC keys
  switch (a_key)
  {
    case SDLK_UNKNOWN:
    case SDLK_RETURN:
    case SDLK_ESCAPE:
    case SDLK_BACKSPACE:
    case SDLK_TAB:
    case SDLK_SPACE:
    case SDLK_EXCLAIM:
    case SDLK_QUOTEDBL:
    case SDLK_HASH:
    case SDLK_PERCENT:
    case SDLK_DOLLAR:
    case SDLK_AMPERSAND:
    case SDLK_QUOTE:
    case SDLK_LEFTPAREN:
    case SDLK_RIGHTPAREN:
    case SDLK_ASTERISK:
    case SDLK_PLUS:
    case SDLK_COMMA:
    case SDLK_MINUS:
    case SDLK_PERIOD:
    case SDLK_SLASH:
    case SDLK_0:
    case SDLK_1:
    case SDLK_2:
    case SDLK_3:
    case SDLK_4:
    case SDLK_5:
    case SDLK_6:
    case SDLK_7:
    case SDLK_8:
    case SDLK_9:
    case SDLK_COLON:
    case SDLK_SEMICOLON:
    case SDLK_LESS:
    case SDLK_EQUALS:
    case SDLK_GREATER:
    case SDLK_QUESTION:
    case SDLK_AT:
    case SDLK_LEFTBRACKET:
    case SDLK_BACKSLASH:
    case SDLK_RIGHTBRACKET:
    case SDLK_CARET:
    case SDLK_UNDERSCORE:
    case SDLK_BACKQUOTE:
    case SDLK_a:
    case SDLK_b:
    case SDLK_c:
    case SDLK_d:
    case SDLK_e:
    case SDLK_f:
    case SDLK_g:
    case SDLK_h:
    case SDLK_i:
    case SDLK_j:
    case SDLK_k:
    case SDLK_l:
    case SDLK_m:
    case SDLK_n:
    case SDLK_o:
    case SDLK_p:
    case SDLK_q:
    case SDLK_r:
    case SDLK_s:
    case SDLK_t:
    case SDLK_u:
    case SDLK_v:
    case SDLK_w:
    case SDLK_x:
    case SDLK_y:
    case SDLK_z:
    case SDLK_CAPSLOCK:
    case SDLK_F1:
    case SDLK_F2:
    case SDLK_F3:
    case SDLK_F4:
    case SDLK_F5:
    case SDLK_F6:
    case SDLK_F7:
    case SDLK_F8:
    case SDLK_F9:
    case SDLK_F10:
    case SDLK_F11:
    case SDLK_F12:
    case SDLK_PRINTSCREEN:
    case SDLK_SCROLLLOCK:
    case SDLK_PAUSE: 
    case SDLK_INSERT:
    case SDLK_HOME:
    case SDLK_PAGEUP:
    case SDLK_DELETE:
    case SDLK_END:
    case SDLK_PAGEDOWN:
    case SDLK_RIGHT:
    case SDLK_LEFT:
    case SDLK_DOWN:
    case SDLK_UP:
    case SDLK_NUMLOCKCLEAR:
    case SDLK_KP_DIVIDE:
    case SDLK_KP_MULTIPLY:
    case SDLK_KP_MINUS:
    case SDLK_KP_PLUS:
    case SDLK_KP_ENTER:
    case SDLK_KP_1:
    case SDLK_KP_2:
    case SDLK_KP_3:
    case SDLK_KP_4:
    case SDLK_KP_5:
    case SDLK_KP_6:
    case SDLK_KP_7:
    case SDLK_KP_8:
    case SDLK_KP_9:
    case SDLK_KP_0:
    case SDLK_KP_PERIOD:
    case SDLK_KP_EQUALS:
    case SDLK_F13:
    case SDLK_F14:
    case SDLK_F15:
    case SDLK_F16:
    case SDLK_F17:
    case SDLK_F18:
    case SDLK_F19:
    case SDLK_F20:
    case SDLK_F21:
    case SDLK_F22:
    case SDLK_F23:
    case SDLK_F24:
    case SDLK_KP_COMMA:
    case SDLK_LCTRL:
    case SDLK_LSHIFT:
    case SDLK_LALT:
    case SDLK_LGUI:
    case SDLK_RCTRL:
    case SDLK_RSHIFT:
    case SDLK_RALT:
    case SDLK_RGUI:
      return a_key;
    default:
      return KC_UNKNOWN;
  }
}

static uint32_t TranslateMouseButtonCode(int a_code)
{
  switch (a_code)
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return a_code;
    default:
      return MC_UNKNOWN;
  }
}

static uint32_t TranslateKeyState(int a_code)
{
  switch (a_code)
  {
    case SDL_KEYUP:
    case SDL_MOUSEBUTTONUP:
      return MT_ButtonUp;
    case SDL_KEYDOWN:
    case SDL_MOUSEBUTTONDOWN:
      return MT_ButtonDown;
    default:
      return MT_None;
  }
}

static uint32_t TranslateWindowCode(int a_code)
{
  //SDL window codes directly map onto our codes

  switch (a_code)
  {
    case  SDL_WINDOWEVENT_SHOWN:
      return MT_Window_Shown;
    case  SDL_WINDOWEVENT_HIDDEN:
      return MT_Window_Hidden;
    case  SDL_WINDOWEVENT_EXPOSED:
      return MT_Window_Exposed;
    case  SDL_WINDOWEVENT_MOVED:
      return MT_Window_Moved;
    case  SDL_WINDOWEVENT_RESIZED:
      return MT_Window_Resized;
    case  SDL_WINDOWEVENT_SIZE_CHANGED:
      return MT_Window_Sized_Changed;
    case  SDL_WINDOWEVENT_MINIMIZED:
      return MT_Window_Minimized;
    case  SDL_WINDOWEVENT_MAXIMIZED:
      return MT_Window_Maximized;
    case  SDL_WINDOWEVENT_RESTORED:
      return MT_Window_Restored;
    case  SDL_WINDOWEVENT_ENTER:
      return MT_Window_Enter;
    case  SDL_WINDOWEVENT_LEAVE:
      return MT_Window_Leave;
    case  SDL_WINDOWEVENT_FOCUS_GAINED:
      return MT_Window_Focus_Gained;
    case  SDL_WINDOWEVENT_FOCUS_LOST:
      return MT_Window_Focus_Lost;
    case  SDL_WINDOWEVENT_CLOSE:
      return MT_Window_Close;
    case  SDL_WINDOWEVENT_TAKE_FOCUS:
      return MT_Window_Take_Focus;
    case  SDL_WINDOWEVENT_HIT_TEST:
      return MT_Window_Hit_Test;
    default:
      return MT_None;
  }

  return uint32_t(a_code);
}

bool FW_EventPoller::NextEvent(Message & a_message)
{
  SDL_Event event;
  if (SDL_PollEvent(&event) == 0)
    return false;

  switch (event.type)
  {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
      a_message.SetType((MC_Input | MC_Keyboard), TranslateKeyState(event.type));
      a_message.key.code = TranslateKeyCode(event.key.keysym.sym);
      a_message.key.repeat = event.key.repeat != 0;
      break;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
      a_message.SetType((MC_Input | MC_Mouse), TranslateKeyState(event.type));
      a_message.mouseButton.code = TranslateMouseButtonCode(event.button.button);
      a_message.mouseButton.x = event.button.x;
      a_message.mouseButton.y = event.button.y;
      break;
    }
    case SDL_MOUSEWHEEL:
    {
      a_message.SetType((MC_Input | MC_Mouse), MT_MouseWheel);
      a_message.mouseWheel.y = event.wheel.y;
      break;
    }
    case SDL_MOUSEMOTION:
    {
      a_message.SetType((MC_Input | MC_Mouse), MT_MouseMotion);
      a_message.mouseMove.x = event.motion.x;
      a_message.mouseMove.y = event.motion.y;
      a_message.mouseMove.xRel = event.motion.xrel;
      a_message.mouseMove.yRel = event.motion.yrel;
      break;
    }
    case SDL_WINDOWEVENT:
    {
      //Handle and send to window system
      a_message.SetType(MC_Window, TranslateWindowCode(event.window.type));
      a_message.window.data1 = event.window.data1;
      a_message.window.data2 = event.window.data2;
      break;
    }
    case SDL_QUIT:
    {
      //Handle quit request
      a_message.SetType(MC_Window, MT_Window_Close);
      break;
    }
    default:
    {
      break;
    }
  }

  return true;
}
