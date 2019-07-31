#include "LayerStack.h"
#include "Layer.h"

LayerStack::LayerStack()
  : m_nextID(1)
{

}

LayerStack::~LayerStack()
{
  Clear();
}

LayerStack::LayerID LayerStack::PushLayer(Layer * a_layer)
{
  m_layerStack.insert(m_nextID, a_layer);
  LayerID val = m_nextID;
  m_nextID++;
  return val;
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