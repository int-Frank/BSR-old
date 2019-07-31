#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "DgMap_AVL.h"

class Layer;

class LayerStack
{
public:

  typedef unsigned LayerID;

public:

  LayerStack();
  ~LayerStack();

  LayerID PushLayer(Layer *);
  void PopLayer(LayerID);
  Layer * GetLayer(LayerID);
  void Clear();

  Dg::Map_AVL<LayerID, Layer *>::iterator begin();
  Dg::Map_AVL<LayerID, Layer *>::iterator end();

private:

  Dg::Map_AVL<LayerID, Layer *>  m_layerStack;
  unsigned                       m_nextID;
};

#endif
