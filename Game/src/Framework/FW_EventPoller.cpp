#include <sstream>

#include "Framework.h"
#include "../Message.h"
#include "../InputCodes.h"
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

private:

  void UpdateModState(SDL_Event const &);

private:

  uint32_t m_modState;
};

void Framework::InitEventPoller()
{
  SetEventPoller(new FW_EventPoller());
}

FW_EventPoller::FW_EventPoller(): 
  m_modState(KM_NONE)
{

}

FW_EventPoller::~FW_EventPoller()
{

}

void FW_EventPoller::UpdateModState(SDL_Event const & a_event)
{
  if (a_event.type == SDL_KEYDOWN)
  {
    if (a_event.key.keysym.sym == SDLK_LSHIFT)            m_modState |= KM_LSHIFT;
    else if (a_event.key.keysym.sym == SDLK_LCTRL)        m_modState |= KM_LCTRL;
    else if (a_event.key.keysym.sym == SDLK_LALT)         m_modState |= KM_LALT;
    else if (a_event.key.keysym.sym == SDLK_LGUI)         m_modState |= KM_LGUI;
    else if (a_event.key.keysym.sym == SDLK_RSHIFT)       m_modState |= KM_RSHIFT;
    else if (a_event.key.keysym.sym == SDLK_RCTRL)        m_modState |= KM_RCTRL;
    else if (a_event.key.keysym.sym == SDLK_RALT)         m_modState |= KM_RALT;
    else if (a_event.key.keysym.sym == SDLK_RGUI)         m_modState |= KM_RGUI;
    else if (a_event.key.keysym.sym == SDLK_NUMLOCKCLEAR) m_modState |= KM_NUM;
    else if (a_event.key.keysym.sym == SDLK_CAPSLOCK)     m_modState |= KM_CAPS;
    else if (a_event.key.keysym.sym == SDLK_MODE)         m_modState |= KM_MODE;
  }
  else if (a_event.type == SDL_KEYUP)
  {
    if (a_event.key.keysym.sym == SDLK_LSHIFT)            m_modState &= ~KM_LSHIFT;
    else if (a_event.key.keysym.sym == SDLK_LCTRL)        m_modState &= ~KM_LCTRL;
    else if (a_event.key.keysym.sym == SDLK_LALT)         m_modState &= ~KM_LALT;
    else if (a_event.key.keysym.sym == SDLK_LGUI)         m_modState &= ~KM_LGUI;
    else if (a_event.key.keysym.sym == SDLK_RSHIFT)       m_modState &= ~KM_RSHIFT;
    else if (a_event.key.keysym.sym == SDLK_RCTRL)        m_modState &= ~KM_RCTRL;
    else if (a_event.key.keysym.sym == SDLK_RALT)         m_modState &= ~KM_RALT;
    else if (a_event.key.keysym.sym == SDLK_RGUI)         m_modState &= ~KM_RGUI;
    else if (a_event.key.keysym.sym == SDLK_NUMLOCKCLEAR) m_modState &= ~KM_NUM;
    else if (a_event.key.keysym.sym == SDLK_CAPSLOCK)     m_modState &= ~KM_CAPS;
    else if (a_event.key.keysym.sym == SDLK_MODE)         m_modState &= ~KM_MODE;
  }
}

static uint32_t TranslateKeyCode(SDL_Keycode a_key)
{
  if (a_key < 128)
    return a_key;

  switch (a_key)
  {
    case SDLK_UNKNOWN:
      return IC_UNKNOWN;
    case SDLK_CAPSLOCK:
      return IC_CAPSLOCK;
    case SDLK_F1:
      return IC_F1;
    case SDLK_F2:
      return IC_F2;
    case SDLK_F3:
      return IC_F3;
    case SDLK_F4:
      return IC_F4;
    case SDLK_F5:
      return IC_F5;
    case SDLK_F6:
      return IC_F6;
    case SDLK_F7:
      return IC_F7;
    case SDLK_F8:
      return IC_F8;
    case SDLK_F9:
      return IC_F9;
    case SDLK_F10:
      return IC_F10;
    case SDLK_F11:
      return IC_F11;
    case SDLK_F12:
      return IC_F12;
    case SDLK_PRINTSCREEN:
      return IC_PRINTSCREEN;
    case SDLK_SCROLLLOCK:
      return IC_SCROLLLOCK;
    case SDLK_PAUSE: 
      return IC_PAUSE;
    case SDLK_INSERT:
      return IC_INSERT;
    case SDLK_HOME:
      return IC_HOME;
    case SDLK_PAGEUP:
      return IC_PAGEUP;
    case SDLK_DELETE:
      return IC_DELETE;
    case SDLK_END:
      return IC_END;
    case SDLK_PAGEDOWN:
      return IC_PAGEDOWN;
    case SDLK_RIGHT:
      return IC_RIGHT;
    case SDLK_LEFT:
      return IC_LEFT;
    case SDLK_DOWN:
      return IC_DOWN;
    case SDLK_UP:
      return IC_UP;
    case SDLK_NUMLOCKCLEAR:
      return IC_NUMLOCKCLEAR;
    case SDLK_KP_DIVIDE:
      return IC_KP_DIVIDE;
    case SDLK_KP_MULTIPLY:
      return IC_KP_MULTIPLY;
    case SDLK_KP_MINUS:
      return IC_KP_MINUS;
    case SDLK_KP_PLUS:
      return IC_KP_PLUS;
    case SDLK_KP_ENTER:
      return IC_KP_ENTER;
    case SDLK_KP_1:
      return IC_KP_1;
    case SDLK_KP_2:
      return IC_KP_2;
    case SDLK_KP_3:
      return IC_KP_3;
    case SDLK_KP_4:
      return IC_KP_4;
    case SDLK_KP_5:
      return IC_KP_5;
    case SDLK_KP_6:
      return IC_KP_6;
    case SDLK_KP_7:
      return IC_KP_7;
    case SDLK_KP_8:
      return IC_KP_8;
    case SDLK_KP_9:
      return IC_KP_9;
    case SDLK_KP_0:
      return IC_KP_0;
    case SDLK_KP_PERIOD:
      return IC_KP_PERIOD;
    case SDLK_KP_EQUALS:
      return IC_KP_EQUALS;
    case SDLK_F13:
      return IC_F13;
    case SDLK_F14:
      return IC_F14;
    case SDLK_F15:
      return IC_F15;
    case SDLK_F16:
      return IC_F16;
    case SDLK_F17:
      return IC_F17;
    case SDLK_F18:
      return IC_F18;
    case SDLK_F19:
      return IC_F19;
    case SDLK_F20:
      return IC_F20;
    case SDLK_F21:
      return IC_F21;
    case SDLK_F22:
      return IC_F22;
    case SDLK_F23:
      return IC_F23;
    case SDLK_F24:
      return IC_F24;
    case SDLK_KP_COMMA:
      return IC_KP_COMMA;
    case SDLK_LCTRL:
      return IC_LCTRL;
    case SDLK_LSHIFT:
      return IC_LSHIFT;
    case SDLK_LALT:
      return IC_LALT;
    case SDLK_LGUI:
      return IC_LGUI;
    case SDLK_RCTRL:
      return IC_RCTRL;
    case SDLK_RSHIFT:
      return IC_RSHIFT;
    case SDLK_RALT:
      return IC_RALT;
    case SDLK_RGUI:
      return IC_RGUI;
    default:
      return IC_UNKNOWN;
  }
}

static uint32_t TranslateMouseButtonCode(int a_code)
{
  switch (a_code)
  {
    case SDL_BUTTON_LEFT:
      return IC_MOUSEBUTTON_LEFT;
    case SDL_BUTTON_MIDDLE:
      return IC_MOUSEBUTTON_MIDDLE;
    case SDL_BUTTON_RIGHT:
      return IC_MOUSEBUTTON_RIGHT;
    case SDL_BUTTON_X1:
      return IC_MOUSEBUTTON_X1;
    case SDL_BUTTON_X2:
      return IC_MOUSEBUTTON_X2;
    default:
      return IC_UNKNOWN;
  }
}

static uint32_t TranslateKeyState(int a_code)
{
  switch (a_code)
  {
    case SDL_KEYUP:
      return MT_KeyUp;
    case SDL_MOUSEBUTTONUP:
      return MT_ButtonUp;
    case SDL_KEYDOWN:
      return MT_KeyUp;
    case SDL_MOUSEBUTTONDOWN:
      return MT_ButtonDown;
    default:
      return MT_None;
  }
}

static uint32_t TranslateWindowCode(int a_code)
{
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
      UpdateModState(event);
      a_message.type = TranslateKeyState(event.type);
      a_message.key.code = m_modState | TranslateKeyCode(event.key.keysym.sym);
      a_message.key.repeat = event.key.repeat != 0;
      break;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
      UpdateModState(event);
      a_message.type = TranslateKeyState(event.type);
      a_message.mouse.code = m_modState | TranslateMouseButtonCode(event.button.button);
      a_message.mouse.x = event.button.x;
      a_message.mouse.y = event.button.y;
      break;
    }
    case SDL_MOUSEWHEEL:
    {
      UpdateModState(event);
      if (event.wheel.y > 0)
      {
        a_message.type = MT_OtherMouseEvent;
        a_message.mouse.code = m_modState | IC_MOUSEWHEEL_UP;
      }
      else
      {
        a_message.type = MT_OtherMouseEvent;
        a_message.mouse.code = m_modState | IC_MOUSEWHEEL_DOWN;
      }
      break;
    }
    case SDL_MOUSEMOTION:
    {
      a_message.type = MT_OtherMouseEvent;
      a_message.mouse.code = IC_MOUSE_MOTION;
      if (SDL_GetRelativeMouseMode() == SDL_TRUE)
      {
        a_message.mouse.x = event.motion.xrel;
        a_message.mouse.y = event.motion.yrel;
      }
      else
      {
        a_message.mouse.x = event.motion.x;
        a_message.mouse.y = event.motion.y;
      }
      break;
    }
    case SDL_WINDOWEVENT:
    {
      //Handle and send to window system
      a_message.type = TranslateWindowCode(event.window.type);
      a_message.window.data1 = event.window.data1;
      a_message.window.data2 = event.window.data2;
      break;
    }
    case SDL_QUIT:
    {
      //Handle quit request
      a_message.type = MT_Window_Close;
      break;
    }
    default:
    {
      break;
    }
  }

  return true;
}
