#include <iostream>

#include "TestHarness.h"
#include "Engine/Memory.h"
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

class TestClassWithID : public Engine::HasResourceID
{
public:
  ~TestClassWithID()
  {
    gVal++;
  }
};

TEST(Stack_ResourceNoID, creation_ResourceNoID)
{
  gVal = 0;
  {
    Engine::Ref<TestClass> refTC(new TestClass());
  }
  CHECK(gVal == 1);
}

TEST(Stack_ResourceWithID, creation_ResourceWithID)
{
  gVal = 0;
  {
    Engine::Ref<TestClassWithID> refTCWithID(new TestClassWithID());
    CHECK(!refTCWithID->GetRefID().IsNull());
  }
  CHECK(gVal == 1);
}

TEST(Stack_User, creation_User)
{
  CHECK(Engine::RegisterResource(42, new TestClassWithID()));
  {
    Engine::Ref<TestClassWithID> refTCWithID(42);
    CHECK(!refTCWithID.IsNull());
  }

  gVal = 0;
  Engine::DestroyResource(42);
  CHECK(gVal == 1);
}