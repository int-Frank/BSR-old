#include "LayerStack.h"
#include "Layer.h"

namespace Engine
{
  LayerStack::LayerStack()
  {

  }

  LayerStack::~LayerStack()
  {
    Clear();
  }

  bool LayerStack::PushLayer(Layer * a_layer, LayerID a_ID)
  {
    if (m_layerStack.find(a_ID) != m_layerStack.end())
      return false;

    m_layerStack.insert(a_ID, a_layer);
    return true;
  }

  void LayerStack::PopLayer(LayerID a_ID)
  {
    auto it = m_layerStack.find(a_ID);
    if (it != m_layerStack.end())
    {
      delete it->second;
      m_layerStack.erase(it);
    }
  }

  void LayerStack::Clear()
  {
    for (auto kv : m_layerStack)
      delete kv.second;
    m_layerStack.clear();
  }

  Layer * LayerStack::GetLayer(LayerID a_ID)
  {
    Layer * result(nullptr);
    auto it = m_layerStack.find(a_ID);
    if (it != m_layerStack.end())
      result = it->second;
    return result;
  }

  Dg::Map_AVL<LayerStack::LayerID, Layer *>::iterator LayerStack::begin()
  {
    return m_layerStack.begin();
  }

  Dg::Map_AVL<LayerStack::LayerID, Layer *>::iterator LayerStack::end()
  {
    return m_layerStack.end();
  }
}