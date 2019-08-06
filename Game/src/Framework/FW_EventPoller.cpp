#include <sstream>
#include <cstring>

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

  void UpdateModState(SDL_Event const & a_event);

private:

  uint16_t m_modState;
};

void FW_EventPoller::UpdateModState(SDL_Event const & a_event)
{
  if (a_event.type == SDL_KEYDOWN)
  {
    switch (a_event.key.keysym.sym)
    {
      case SDLK_LSHIFT:       m_modState |= KM_LSHIFT; break;
      case SDLK_LCTRL:        m_modState |= KM_LCTRL; break;
      case SDLK_LALT:         m_modState |= KM_LALT; break;
      case SDLK_LGUI:         m_modState |= KM_LGUI; break;
      case SDLK_RSHIFT:       m_modState |= KM_RSHIFT; break;
      case SDLK_RCTRL:        m_modState |= KM_RCTRL; break;
      case SDLK_RALT:         m_modState |= KM_RALT; break;
      case SDLK_RGUI:         m_modState |= KM_RGUI; break;
      case SDLK_NUMLOCKCLEAR: m_modState &= ~KM_NUM; break;
      case SDLK_CAPSLOCK:     m_modState &= ~KM_CAPS; break;
      case SDLK_MODE:         m_modState &= ~KM_MODE; break;
      default: break;
    }
  }
  else if (a_event.type == SDL_KEYUP)
  {
    switch (a_event.key.keysym.sym)
    {
      case SDLK_LSHIFT:       m_modState &= ~KM_LSHIFT; break;
      case SDLK_LCTRL:        m_modState &= ~KM_LCTRL; break;
      case SDLK_LALT:         m_modState &= ~KM_LALT; break;
      case SDLK_LGUI:         m_modState &= ~KM_LGUI; break;
      case SDLK_RSHIFT:       m_modState &= ~KM_RSHIFT; break;
      case SDLK_RCTRL:        m_modState &= ~KM_RCTRL; break;
      case SDLK_RALT:         m_modState &= ~KM_RALT; break;
      case SDLK_RGUI:         m_modState &= ~KM_RGUI; break;
      default: break;
    }
  }
}

void Framework::InitEventPoller()
{
  SetEventPoller(new FW_EventPoller());
}

FW_EventPoller::FW_EventPoller()
  : m_modState(0)
{
  SDL_Keymod modState = SDL_GetModState();
  if ((modState & KMOD_CAPS) != 0)
    m_modState |= KM_CAPS;
  if ((modState & KMOD_NUM) != 0)
    m_modState |= KM_NUM;
}

FW_EventPoller::~FW_EventPoller()
{

}

static uint32_t TranslateKeyCode(SDL_Scancode a_key)
{
  switch (a_key)
  {
    case SDL_SCANCODE_UNKNOWN:
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_B:
    case SDL_SCANCODE_C:
    case SDL_SCANCODE_D:
    case SDL_SCANCODE_E:
    case SDL_SCANCODE_F:
    case SDL_SCANCODE_G:
    case SDL_SCANCODE_H:
    case SDL_SCANCODE_I:
    case SDL_SCANCODE_J:
    case SDL_SCANCODE_K:
    case SDL_SCANCODE_L:
    case SDL_SCANCODE_M:
    case SDL_SCANCODE_N:
    case SDL_SCANCODE_O:
    case SDL_SCANCODE_P:
    case SDL_SCANCODE_Q:
    case SDL_SCANCODE_R:
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_T:
    case SDL_SCANCODE_U:
    case SDL_SCANCODE_V:
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_X:
    case SDL_SCANCODE_Y:
    case SDL_SCANCODE_Z:
    case SDL_SCANCODE_1:
    case SDL_SCANCODE_2:
    case SDL_SCANCODE_3:
    case SDL_SCANCODE_4:
    case SDL_SCANCODE_5:
    case SDL_SCANCODE_6:
    case SDL_SCANCODE_7:
    case SDL_SCANCODE_8:
    case SDL_SCANCODE_9:
    case SDL_SCANCODE_0:
    case SDL_SCANCODE_RETURN:
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_BACKSPACE:
    case SDL_SCANCODE_TAB:
    case SDL_SCANCODE_SPACE:
    case SDL_SCANCODE_MINUS:
    case SDL_SCANCODE_EQUALS:
    case SDL_SCANCODE_LEFTBRACKET:
    case SDL_SCANCODE_RIGHTBRACKET:
    case SDL_SCANCODE_BACKSLASH:
    case SDL_SCANCODE_NONUSHASH:
    case SDL_SCANCODE_SEMICOLON:
    case SDL_SCANCODE_APOSTROPHE:
    case SDL_SCANCODE_GRAVE:
    case SDL_SCANCODE_COMMA:
    case SDL_SCANCODE_PERIOD:
    case SDL_SCANCODE_SLASH:
    case SDL_SCANCODE_CAPSLOCK:
    case SDL_SCANCODE_F1:
    case SDL_SCANCODE_F2:
    case SDL_SCANCODE_F3:
    case SDL_SCANCODE_F4:
    case SDL_SCANCODE_F5:
    case SDL_SCANCODE_F6:
    case SDL_SCANCODE_F7:
    case SDL_SCANCODE_F8:
    case SDL_SCANCODE_F9:
    case SDL_SCANCODE_F10:
    case SDL_SCANCODE_F11:
    case SDL_SCANCODE_F12:
    case SDL_SCANCODE_PRINTSCREEN:
    case SDL_SCANCODE_SCROLLLOCK:
    case SDL_SCANCODE_PAUSE:
    case SDL_SCANCODE_INSERT:
    case SDL_SCANCODE_HOME:
    case SDL_SCANCODE_PAGEUP:
    case SDL_SCANCODE_DELETE:
    case SDL_SCANCODE_END:
    case SDL_SCANCODE_PAGEDOWN:
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_NUMLOCKCLEAR:
    case SDL_SCANCODE_KP_DIVIDE:
    case SDL_SCANCODE_KP_MULTIPLY:
    case SDL_SCANCODE_KP_MINUS:
    case SDL_SCANCODE_KP_PLUS:
    case SDL_SCANCODE_KP_ENTER:
    case SDL_SCANCODE_KP_1:
    case SDL_SCANCODE_KP_2:
    case SDL_SCANCODE_KP_3:
    case SDL_SCANCODE_KP_4:
    case SDL_SCANCODE_KP_5:
    case SDL_SCANCODE_KP_6:
    case SDL_SCANCODE_KP_7:
    case SDL_SCANCODE_KP_8:
    case SDL_SCANCODE_KP_9:
    case SDL_SCANCODE_KP_0:
    case SDL_SCANCODE_KP_PERIOD:
    case SDL_SCANCODE_NONUSBACKSLASH:
    case SDL_SCANCODE_APPLICATION:
    case SDL_SCANCODE_KP_EQUALS:
    case SDL_SCANCODE_F13:
    case SDL_SCANCODE_F14:
    case SDL_SCANCODE_F15:
    case SDL_SCANCODE_F16:
    case SDL_SCANCODE_F17:
    case SDL_SCANCODE_F18:
    case SDL_SCANCODE_F19:
    case SDL_SCANCODE_F20:
    case SDL_SCANCODE_F21:
    case SDL_SCANCODE_F22:
    case SDL_SCANCODE_F23:
    case SDL_SCANCODE_F24:
    case SDL_SCANCODE_KP_COMMA:
    case SDL_SCANCODE_KP_EQUALSAS400:
    case SDL_SCANCODE_INTERNATIONAL1:
    case SDL_SCANCODE_INTERNATIONAL2:
    case SDL_SCANCODE_INTERNATIONAL3:
    case SDL_SCANCODE_INTERNATIONAL4:
    case SDL_SCANCODE_INTERNATIONAL5:
    case SDL_SCANCODE_INTERNATIONAL6:
    case SDL_SCANCODE_LANG1:
    case SDL_SCANCODE_LANG2:
    case SDL_SCANCODE_LANG3:
    case SDL_SCANCODE_LANG4:
    case SDL_SCANCODE_LANG5:
    case SDL_SCANCODE_KP_LEFTPAREN:
    case SDL_SCANCODE_KP_RIGHTPAREN:
    case SDL_SCANCODE_KP_LEFTBRACE:
    case SDL_SCANCODE_KP_RIGHTBRACE:
    case SDL_SCANCODE_LCTRL:
    case SDL_SCANCODE_LSHIFT:
    case SDL_SCANCODE_LALT:
    case SDL_SCANCODE_LGUI:
    case SDL_SCANCODE_RCTRL:
    case SDL_SCANCODE_RSHIFT:
    case SDL_SCANCODE_RALT:
    case SDL_SCANCODE_RGUI:
      return a_key;
    default:
      return IC_UNKNOWN;
  }
}

static uint32_t TranslateMouseButtonCode(int a_code)
{
  switch (a_code)
  {
    case SDL_BUTTON_LEFT:
      return IC_MOUSE_BUTTON_LEFT;
    case SDL_BUTTON_MIDDLE:
      return IC_MOUSE_BUTTON_MIDDLE;
    case SDL_BUTTON_RIGHT:
      return IC_MOUSE_BUTTON_RIGHT;
    case SDL_BUTTON_X1:
      return IC_MOUSE_BUTTON_X1;
    case SDL_BUTTON_X2:
      return IC_MOUSE_BUTTON_X2;
    default:
      return IC_UNKNOWN;
  }
}

static uint32_t TranslateKeyState(SDL_Event a_event)
{
  switch (a_event.type)
  {
    case SDL_KEYUP:
      return MT_Input_KeyUp;
    case SDL_MOUSEBUTTONUP:
      return MT_Input_ButtonUp;
    case SDL_KEYDOWN:
      return (a_event.key.repeat == 0 ? MT_Input_KeyDown : MT_Input_KeyDown_Repeat);
    case SDL_MOUSEBUTTONDOWN:
      return MT_Input_ButtonDown;
    default:
      return MT_None;
  }
}

static uint32_t TranslateWindowCode(uint8_t a_code)
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
  bool validEvent = false;
  while (!validEvent)
  {
    SDL_Event event;
    if (SDL_PollEvent(&event) == 0)
      break;

    switch (event.type)
    {
      case SDL_TEXTINPUT:
      {
        a_message.type = MT_Input_Text;
        static_assert(TEXT_INPUT_TEXT_SIZE == SDL_TEXTINPUTEVENT_TEXT_SIZE, "text container incorrect size");
        strncpy_s(a_message.text.text, event.text.text, TEXT_INPUT_TEXT_SIZE);
        validEvent = true;
        break; 
      }
      case SDL_KEYDOWN:
      case SDL_KEYUP:
      {
        UpdateModState(event);
        a_message.type = TranslateKeyState(event);
        a_message.key.code = TranslateKeyCode(event.key.keysym.scancode);
        a_message.key.modState = m_modState;
        validEvent = true;
        break;
      }
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
      {
        a_message.type = TranslateKeyState(event);
        a_message.mouse.code = TranslateMouseButtonCode(event.button.button);
        a_message.mouse.x = event.button.x;
        a_message.mouse.y = event.button.y;
        validEvent = true;
        break;
      }
      case SDL_MOUSEWHEEL:
      {
        if (event.wheel.y > 0)
        {
          a_message.type = MT_Input_OtherMouseEvent;
          a_message.mouse.code = IC_MOUSE_WHEEL_UP;
        }
        else
        {
          a_message.type = MT_Input_OtherMouseEvent;
          a_message.mouse.code = IC_MOUSE_WHEEL_DOWN;
        }
        validEvent = true;
        break;
      }
      case SDL_MOUSEMOTION:
      {
        a_message.type = MT_Input_OtherMouseEvent;
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
        validEvent = true;
        break;
      }
      case SDL_WINDOWEVENT:
      {
        //Handle and send to window system
        a_message.type = TranslateWindowCode(event.window.event);
        a_message.window.data1 = event.window.data1;
        a_message.window.data2 = event.window.data2;
        validEvent = true;
        break;
      }
      case SDL_QUIT:
      {
        //Handle quit request
        a_message.type = MT_Window_Close;
        validEvent = true;
        break;
      }
      default:
      {
        break;
      }
    }
  }

  return validEvent;
}
