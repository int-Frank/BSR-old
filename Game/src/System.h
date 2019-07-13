#ifndef SYSTEM_H
#define SYSTEM_H

class MessageBus;
struct Message;

//TODO Subscribers should collect messages and process them on a single call
//     to Update(). This way, we can throw the subscriber on a separate thread
//     and update.
class System
{
public: 

  System(MessageBus * a_pMsgBus);
  virtual ~System();

  virtual void OnAttach(){};
  virtual void OnDetach(){};
  virtual void ShutDown(){};

  virtual void Update() =0;

  //Return bool: consumed
  virtual bool HandleMessage(Message const &) =0;
  void Post(Message const &);

  //This is dangerous in a multithreaded environment.
  //Try to get away with not having it
  //void PriorityPost(Message const &);

private:

  System(System const &);
  System & operator=(System const &);

  MessageBus * m_pMsgBus;
};

#endif