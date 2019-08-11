#ifndef IEVENTPOLLER_H
#define IEVENTPOLLER_H

namespace Engine
{
  class Message;

  class IEventPoller
  {
  public:

    ~IEventPoller(){}

    //Returns nullptr if no new event
    virtual Message * NextEvent() =0;

  };
}

#endif