#ifndef EN_IEVENTPOLLER_H
#define EN_IEVENTPOLLER_H

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