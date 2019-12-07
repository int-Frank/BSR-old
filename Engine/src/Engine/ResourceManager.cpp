#include "ResourceManager.h"
#include "Resource.h"
#include "core_Assert.h"

namespace Engine
{
  void DestroyResource(ResourceID a_id)
  {
    impl::ResourceID64 id64;
    id64.SetType(impl::T_UserID);
    id64.SetID(a_id);
    impl::ResourceManager::Instance()->DestroyResource(id64);
  }

  namespace impl
  {
    //--------------------------------------------------------------------------------------
    // ResourceBase
    //--------------------------------------------------------------------------------------
    ResourceBase::ResourceBase(void * a_pObj)
      : m_pObj(a_pObj)
    {

    }

    ResourceBase::~ResourceBase()
    {

    }

    void * ResourceBase::GetPointer()
    {
      return m_pObj;
    }

    //--------------------------------------------------------------------------------------
    // Resource Manager
    //--------------------------------------------------------------------------------------
    ResourceManager * ResourceManager::s_instance = nullptr;

    ResourceManager* ResourceManager::Instance()
    {
      if (s_instance == nullptr)
        s_instance = new ResourceManager();
      return s_instance;
    }

    ResourceManager::ResourceManager()
      : m_currentID(0)
    {

    }

    ResourceBase* ResourceManager::GetResource(ResourceID64 a_id, bool a_registerUser)
    {
      std::lock_guard<std::mutex> lck(m_mutex);
      Data * pData = m_resourceMap.at(a_id);
      if (pData == nullptr)
        return nullptr;
      pData->counter += a_registerUser ? 1 : 0;
      return pData->pResource;
    }

    uint32_t ResourceManager::GenerateUnique32()
    {
      return ++m_currentID;
    }

    void ResourceManager::RegisterUser(ResourceID64 a_id)
    {
      std::lock_guard<std::mutex> lck(m_mutex);
      Data* pData = m_resourceMap.at(a_id);
      if (pData != nullptr)
        pData->counter++;
    }

    void ResourceManager::DeregisterUser(ResourceID64 a_id)
    {
      std::lock_guard<std::mutex> lck(m_mutex);
      Data* pData = m_resourceMap.at(a_id);
      if (pData != nullptr)
      {
        BSR_ASSERT(pData->counter > 0, "Attempt to deregister a resource with no registered users!");
        pData->counter--;
        if (pData->counter == 0 && !a_id.Is(ResourceID64::Flag::Persistant))
        {
          delete pData->pResource;
          pData->pResource = nullptr;
          m_resourceMap.erase(a_id);
        }
      }
    }

    void ResourceManager::RegisterResource(ResourceID64 a_id, ResourceBase* a_pRes)
    {
      std::lock_guard<std::mutex> lck(m_mutex);
      BSR_ASSERT(m_resourceMap.at(a_id) == nullptr, "Attempt to register an existing resource!");
      Data data{a_pRes, 0};
      m_resourceMap.insert(a_id, data);
    }

    void ResourceManager::DestroyResource(ResourceID64 a_id)
    {
      std::lock_guard<std::mutex> lck(m_mutex);
      Data* pData = m_resourceMap.at(a_id);
      if (pData != nullptr)
      {
        delete pData->pResource;
        pData->pResource = nullptr;
        m_resourceMap.erase(a_id);
      }
    }
  }
}