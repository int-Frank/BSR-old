#ifndef IMOUSECONTROLLER_H
#define IMOUSECONTROLLER_H

#include "ErrorCodes.h"

class IMouseController
{
public:

  ~IMouseController(){}
  virtual ErrorCode Grab() =0;
  virtual ErrorCode Release() =0;

  virtual void GetPos(int & x, int & y) =0;
  virtual void MoveToPos(int x, int y) =0;

private:

};

#endif