//@group Memory

#pragma once

#include <stdint.h>
#include "DgBit.h"
#include "ResourceManager.h"
#include "ResourceID.h"

namespace Engine
{
  typedef uint32_t ResourceID;

  void DestroyResource(ResourceID a_id);

  //If fail, returns false, resource not touched.
  //If succeed, returns true, ownership of a_obj taken
  template<typename T>
  bool RegisterResource(ResourceID a_id, T* a_obj)
  {
    impl::ResourceID64 id64;
    id64.SetFlag(impl::ResourceID64::Flag::Persistant, true);
    id64.SetType(impl::T_UserID);
    id64.SetID(a_id);
    impl::ResourceWrapperBase * res = impl::ResourceManager::Instance()->GetResource(id64, false);
    if (res == nullptr)
    {
      impl::ResourceManager::Instance()->RegisterResource(id64, new impl::ResourceWrapper<T>(a_obj));
      return true;
    }
    return false;
  }

  //Derive from this class if you want a resource to have the ability to manually
  //register/deregister itself as a resource. This is useful if we need the resource
  //to stick around in memory even after all users have been deregisted. For example,
  //we submit a resource pointer during RENDER_SUBMIT, not a Ref. If the resouce is
  //cleared between when we submit and when the renderer processes the resource, we are
  //left with a dangling pointer. To avoid this, we manually register a user before we 
  //submit, and manually deregister after the renderer processes it.
  /*class Resource
  {
  public:

    virtual ~Resource(){}

    impl::ResourceID64 GetRefID() const
    {
      return m_resourceID;
    }

    void SetRefID(impl::ResourceID64 a_id)
    {
      m_resourceID = a_id;
    }

    void RegisterMe() const
    {
      impl::ResourceManager::Instance()->RegisterUser(m_resourceID);
    }

    void DeregisterMe() const
    {
      impl::ResourceManager::Instance()->DeregisterUser(m_resourceID);
    }

  private:
    impl::ResourceID64 m_resourceID;
  };*/
}