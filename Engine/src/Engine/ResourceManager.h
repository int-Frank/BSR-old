#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <mutex>
#include <atomic>
#include <stdint.h>
#include "DgOpenHashMap.h"
#include "ResourceID.h"

namespace Engine
{
  namespace impl
  {
    enum IDType : uint16_t
    {
      T_UserID    = 1,
      T_Generated = 2
    };

    class ResourceBase
    {
    public:

      ResourceBase(void*);
      virtual ~ResourceBase();
      void * GetPointer();
    protected:

      void* m_pObj;
    };

    class IDEqualTo
    {
    public:
      bool operator()(impl::ResourceID64 const& a_id0,
                      impl::ResourceID64 const& a_id1) const
      {
        return a_id0.GetID() == a_id1.GetID();
      }
    };

    class IDHasher
    {
    public:
      size_t operator()(impl::ResourceID64 const& a_id) const
      {
        return static_cast<size_t>(a_id.GetID());
      }
    };

    class ResourceManager
    {
    public:

      struct Data
      {
        ResourceBase *  pResource;
        uint32_t        counter;
      };

      //static ResourceID const NULL_ResourceID = 0;

      ResourceManager();

      static ResourceManager* Instance();

      ResourceBase* GetResource(ResourceID64, bool registerUser);
      uint32_t GenerateUnique32();

      void RegisterUser(ResourceID64);
      void DeregisterUser(ResourceID64);

      void RegisterResource(ResourceID64, ResourceBase*);
      void DestroyResource(ResourceID64);

    private:

      static ResourceManager* s_instance;

      std::atomic<uint32_t>               m_currentID;
      std::mutex                          m_mutex;
      Dg::OpenHashMap<ResourceID64, Data, IDHasher, IDEqualTo> m_resourceMap;
    };

    template<typename T>
    class Resource : public ResourceBase
    {
    public:

      Resource();
      Resource(T*);
      ~Resource();
    };

    template<typename T>
    Resource<T>::Resource()
      : ResourceBase(nullptr)
    {

    }

    template<typename T>
    Resource<T>::Resource(T* a_pObj)
      : ResourceBase(a_pObj)
    {

    }

    template<typename T>
    Resource<T>::~Resource()
    {
      delete static_cast<T*>(m_pObj);
      m_pObj = nullptr;
    }
  }
}

#endif