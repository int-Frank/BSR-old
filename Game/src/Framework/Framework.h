#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "ErrorCodes.h"

#include "../IWindow.h"
#include "../IEventPoller.h"

//class Logger;
class IDraw;
class IAudio;

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
  //Logger *    GetLogger();
  IWindow *      GetWindow();
  IEventPoller * GetEventPoller();
  IDraw *        GetDraw();
  IAudio *       GetAudio();

private:

  //These are implemented in the relevent cpp files
  void InitWindow();
  void InitEventPoller();

private:

  void SetWindow(IWindow *);
  void SetEventPoller(IEventPoller *);

private:

  static Framework * s_instance;

  class PIMPL;
  PIMPL * m_pimpl;

};

#endif