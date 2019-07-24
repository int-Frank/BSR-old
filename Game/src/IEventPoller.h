#ifndef IEVENTPOLLER_H
#define IEVENTPOLLER_H

struct Message;

class IEventPoller
{
public:

  ~IEventPoller(){}
  virtual bool NextEvent(Message &) =0;

};

#endif