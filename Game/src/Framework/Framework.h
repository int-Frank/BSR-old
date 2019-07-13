#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "ErrorCodes.h"

//class Logger;
class IWindow;
class IEventPoller;
class IDraw;
class IAudio;

class Framework
{
public:

  Framework();
  ~Framework();

  static Framework * Instance();
  static void DestroyInstance();

  ErrorCode Init();
  ErrorCode ShutDown();

  //These will pass pointers to internal objects.
  //Do NOT delete these! All internal objects will
  //Destroyed on calling DestroyInstance();
  //Logger *    GetLogger();
  IWindow *      GetWindow();
  IEventPoller * GetEventPoller();
  IDraw *        GetDraw();
  IAudio *       GetAudio();

private:

  static Framework * s_instance;

  class PIMPL;
  PIMPL * m_pimpl;

};

#endif