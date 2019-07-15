#ifndef IEVENTPOLLER_H
#define IEVENTPOLLER_H

class Message;

class IEventPoller
{
public:

  ~IEventPoller(){}
  virtual bool NextEvent(Message &) =0;

};

#endif