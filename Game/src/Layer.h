#ifndef LAYER_H
#define LAYER_H

class MessageBus;
struct Message;

//TODO Subscribers should collect messages and process them on a single call
//     to Update(). This way, we can throw the subscriber on a separate thread
//     and update.
class Layer
{
public: 

  Layer(MessageBus * a_pMsgBus);
  virtual ~Layer();

  virtual void OnAttach(){}
  virtual void OnDetach(){}

  virtual void Update() =0;

  //Return bool: consumed
  virtual bool HandleMessage(Message const &) =0;
  void Post(Message const &);

  //This is dangerous in a multithreaded environment.
  //Try to get away with not having it
  //void PriorityPost(Message const &);

private:

  Layer(Layer const &);
  Layer & operator=(Layer const &);

  MessageBus * m_pMsgBus;
};

#endif