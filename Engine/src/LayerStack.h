//@group Layers

#ifndef EN_LAYERSTACK_H
#define EN_LAYERSTACK_H

#include <stdint.h>
#include "DgMap_AVL.h"

namespace Engine
{
  class Layer;

  class LayerStack
  {
  public:

    typedef uint32_t LayerID;

  public:

    LayerStack();
    ~LayerStack();

    bool PushLayer(Layer *, LayerID ID);
    void PopLayer(LayerID);
    Layer * GetLayer(LayerID);
    void Clear();

    Dg::Map_AVL<LayerID, Layer *>::iterator begin();
    Dg::Map_AVL<LayerID, Layer *>::iterator end();

  private:

    Dg::Map_AVL<LayerID, Layer *>  m_layerStack;
  };
}

#endif
