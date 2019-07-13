#ifndef FW_SDLWINDOW_H
#define FW_SDLWINDOW_H

#include "../IWindow.h"

struct SDL_Window;
typedef void *SDL_GLContext;

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

private:

  SDL_Window *  m_window;
  SDL_GLContext m_glContext;
};

#endif