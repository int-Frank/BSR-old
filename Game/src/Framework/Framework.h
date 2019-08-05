#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "ErrorCodes.h"

#include "../IWindow.h"
#include "../IEventPoller.h"
#include "../IMouseController.h"

class Framework
{
public:

  Framework();
  ~Framework();

  static Framework * Instance();
  static ErrorCode Init();
  static ErrorCode ShutDown();

  //These will pass pointers to internal objects.
  //Do NOT delete these! All internal objects will
  //Destroyed on calling DestroyInstance();

  IWindow *      GetWindow();
  IEventPoller * GetEventPoller();
  IMouseController * GetMouseController();

  //IDraw *        GetDraw();
  //IAudio *       GetAudio();

  //These can only be initialized after the window has been created.
  bool InitImGui();

private:

  //These are implemented in the relevent cpp files
  void InitWindow();
  void InitEventPoller();
  void InitMouseController();

private:

  void SetWindow(IWindow *);
  void SetEventPoller(IEventPoller *);
  void SetMouseController(IMouseController *);

private:

  static Framework * s_instance;

  class PIMPL;
  PIMPL * m_pimpl;

};

#endif