#ifndef REF_H
#define REF_H

#include <stdint.h>
#include "ResourceManager.h"
#include "ResourceID.h"
#include "Resource.h"

namespace Engine
{
  template<typename T>
  class Ref
  {
  public:

    Ref();

    // default constructor when T has no SetRefID method
    template
      <
      typename U = T,
      typename = typename std::enable_if< !impl::HasFn_SetRefID<U>::value >::type
      >
      constexpr Ref(T* a_pObj)
      : m_pObject(a_pObj)
    {
      m_id.SetType(impl::T_Generated);
      m_id.SetID(impl::ResourceManager::Instance()->GenerateUnique32());
      impl::ResourceManager::Instance()->RegisterResource(m_id, new impl::Resource<T>(m_pObject));
      impl::ResourceManager::Instance()->RegisterUser(m_id);
    }

    // default constructor when T does have SetRefID method
    template
      <
      typename U = T,
      typename = typename std::enable_if< impl::HasFn_SetRefID<U>::value >::type,
      typename = U
      >
      constexpr Ref(T* a_pObj)
      : m_pObject(a_pObj)
    {
      m_id.SetType(impl::T_Generated);
      m_id.SetID(impl::ResourceManager::Instance()->GenerateUnique32());
      impl::ResourceManager::Instance()->RegisterResource(m_id, new impl::Resource<T>(m_pObject));
      impl::ResourceManager::Instance()->RegisterUser(m_id);
      m_pObject->SetRefID(m_id);
    }

    //Construct from an already registered resource
    Ref(ResourceID a_id);

    ~Ref();

    Ref(Ref const&);
    Ref& operator=(Ref const&);

    T* operator->();
    T& operator*();

    bool IsNull() const;

  private:

    impl::ResourceID64  m_id;
    T* m_pObject;
  };

  template<typename T>
  Ref<T>::Ref()
    : m_pObject(nullptr)
  {

  }

  template<typename T>
  Ref<T>::Ref(ResourceID a_id)
    : m_pObject(nullptr)
  {
    m_id.SetType(impl::T_UserID);
    m_id.SetID(a_id);
    impl::ResourceBase res = impl::ResourceManager::Instance()->GetResource(m_id, true);
    if (res != nullptr)
      m_pObject = static_cast<T*>(res.GetPointer());
    else
      m_id.SetNull();
  }

  template<typename T>
  Ref<T>::~Ref()
  {
    impl::ResourceManager::Instance()->DeregisterUser(m_id);
  }

  template<typename T>
  Ref<T>::Ref(Ref const& a_other)
    : m_id(a_other.m_id)
    , m_pObject(a_other.m_pObject)
  {
    impl::ResourceManager::Instance()->RegisterUser(m_id);
  }

  template<typename T>
  Ref<T>& Ref<T>::operator=(Ref const& a_other)
  {
    if (this != &a_other)
    {
      impl::ResourceManager::Instance()->DeregisterUser(m_id);
      m_id = a_other.m_id;
      m_pObject = a_other.m_pObject;
      impl::ResourceManager::Instance()->RegisterUser(m_id);
    }
    return *this;
  }

  template<typename T>
  T* Ref<T>::operator->()
  {
    return m_pObject;
  }

  template<typename T>
  T& Ref<T>::operator*()
  {
    return *m_pObject;
  }

  template<typename T>
  bool Ref<T>::IsNull() const
  {
    return m_pObject == nullptr;
  }
}

#endif