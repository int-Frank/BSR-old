//@group Layers

#ifndef EN_LAYERSTACK_H
#define EN_LAYERSTACK_H

#include <stdint.h>
#include "DgMap_AVL.h"
#include "Layer.h"

namespace Engine
{
  class Layer;

  class LayerStack
  {
  public:

    LayerStack();
    ~LayerStack();

    bool PushLayer(Layer *, Layer::ID);
    void PopLayer(Layer::ID);
    Layer * GetLayer(Layer::ID);
    void Clear();

    Dg::Map_AVL<Layer::ID, Layer *>::iterator begin();
    Dg::Map_AVL<Layer::ID, Layer *>::iterator end();

  private:

    Dg::Map_AVL<Layer::ID, Layer *>  m_layerStack;
  };
}

#endif
