#pragma once

#include <stdint.h>
#include "DgBit.h"
#include "ResourceManager.h"
#include "ResourceID.h"

namespace Engine
{
  typedef uint32_t ResourceID;

  void InitResourceManager();
  void ShutDownResourceManager();

  void DestroyResource(ResourceID a_id);

  //If fail, returns false, resource not touched.
  //If succeed, returns true, ownership of a_obj taken
  template<typename T>
  void RegisterResource(ResourceID a_id, T* a_obj)
  {
    impl::ResourceID64 id64;
    m_id.SetType(impl::T_UserID);
    m_id.SetID(a_id);
    if (impl::ResourceBase res = impl::ResourceManager::Instance()->GetResource(id64, false) != nullptr)
    {
      impl::ResourceManager::Instance()->RegisterResource(id64, new impl::Resource<T>(a_obj));
      return true;
    }
    return false;
  }
}