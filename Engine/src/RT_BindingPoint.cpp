#include <glad/glad.h>

#include "RT_BindingPoint.h"
#include "core_Assert.h"
#include "core_Log.h"

namespace Engine
{
  RT_BindingPoint::Domain RT_BindingPoint::s_addresses[SBT_COUNT][ShaderDomain::SD_COUNT];

  //------------------------------------------------------------------------------------------------
  // BindingPointID
  //------------------------------------------------------------------------------------------------

  BindingPointID::BindingPointID()
    : m_data(INVALID_ID)
  {
    SetInvalid();
  }

  bool BindingPointID::IsValid() const
  {
    return m_data != INVALID_ID;
  }

  uint32_t BindingPointID::Address() const
  {
    return m_data & 0xFFFF;
  }

  ShaderDomain BindingPointID::Domain() const
  {
    return static_cast<ShaderDomain>((m_data >> 16) & 0xFF);
  }

  StorageBlockType BindingPointID::Type() const
  {
    return static_cast<StorageBlockType>((m_data >> 24) & 0xFF);
  }

  void BindingPointID::SetInvalid()
  {
    m_data = INVALID_ID;
  }

  void BindingPointID::Set(StorageBlockType a_type,
                           ShaderDomain a_domain,
                           uint32_t a_index)
  {
    BSR_ASSERT(a_index < 0xFFFF, "Index to high!");
    BSR_ASSERT(a_domain < SD_COUNT, "Invalid Domain!");
    BSR_ASSERT(a_type < SBT_COUNT, "Invalid Storage block type!");

    m_data = a_index;
    m_data = m_data | (a_domain << 16);
    m_data = m_data | (a_type << 24);
  }

  //------------------------------------------------------------------------------------------------
  // RT_BindingPoint
  //------------------------------------------------------------------------------------------------

  void RT_BindingPoint::Init()
  {
    int values[SBT_COUNT][ShaderDomain::SD_COUNT];

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &values[SBT_Uniform][SD_Vertex]);
    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &values[SBT_Uniform][SD_Geometry]);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &values[SBT_Uniform][SD_Fragment]);

    glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &values[SBT_ShaderStorage][SD_Vertex]);
    glGetIntegerv(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, &values[SBT_ShaderStorage][SD_Geometry]);
    glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &values[SBT_ShaderStorage][SD_Fragment]);

    for (int i = 0; i < SBT_COUNT; i++)
    {
      uint16_t begin = 0;
      for (int j = 0; j < SD_COUNT; j++)
      {
        s_addresses[i][j].begin = begin;
        s_addresses[i][j].count = values[i][j];
        begin += values[i][j];
      }
    }
  }

  bool RT_BindingPoint::Capture(StorageBlockType a_type, ShaderDomain a_domain)
  {
    Release();

    for (uint16_t i = 0; i < s_addresses[a_type][a_domain].count; i++)
    {
      uint32_t val = (1 << i) & s_addresses[a_type][a_domain].bindingPoints;
      if (val == 0)
      {
        uint32_t index = s_addresses[a_type][a_domain].begin + i;
        m_bindingIndex.Set(a_type, a_domain, index);
        s_addresses[a_type][a_domain].bindingPoints = s_addresses[a_type][a_domain].bindingPoints | (1 << i);
        return true;
      }
    }
    LOG_WARN("Binding point failed to capture a value!");
    return false;
  }

  void RT_BindingPoint::Release()
  {
    uint32_t index = m_bindingIndex.Address() - s_addresses[m_bindingIndex.Type()][m_bindingIndex.Domain()].begin;
    s_addresses[m_bindingIndex.Type()][m_bindingIndex.Domain()].bindingPoints =
      s_addresses[m_bindingIndex.Type()][m_bindingIndex.Domain()].bindingPoints & ~(1 << index);
  }

  bool RT_BindingPoint::IsBound() const
  {
    return m_bindingIndex.IsValid();
  }

  BindingPointID RT_BindingPoint::GetID() const
  {
    return m_bindingIndex;
  }
}