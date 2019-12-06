#include "TestHarness.h"
#include "Engine/Ref.h"
#include "Engine/Resource.h"

static int gVal = 0;

class TestClass
{
public:

  ~TestClass()
  {
    gVal++;
  }
};

class TestClassWithID
{
  ALLOW_LINK_TO_REF
public:
  ~TestClassWithID()
  {
    gVal++;
  }
};

TEST(Stack_Resource, creation_Resource)
{
  Engine::InitResourceManager();

  {
    Engine::impl::HasFn_SetRefID<TestClass>::value;
    Engine::impl::HasFn_SetRefID<TestClassWithID>::value;

    Engine::Ref<TestClass> refTC(new TestClass());
    Engine::Ref<TestClassWithID> refTCWithID(new TestClassWithID());
  }

  Engine::ShutDownResourceManager();
}