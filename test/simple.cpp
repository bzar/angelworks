#include "angelworksstate.h"
#include "angelworksservice.h"
#include "ew/engine.h"
#include <iostream>

class TestService : public AngelWorksService
{
public:
  struct Event1
  {
    static int ID;
    int foo;
  };

  ~TestService()
  {
    for(asIScriptObject* obj : registered)
    {
      obj->Release();
    }
    registered.clear();
  }

  void registerToEngine(asIScriptEngine* e)
  {
    engine = e;
    Event1::ID = engine->RegisterObjectType("Event1", sizeof(Event1), asOBJ_VALUE | asOBJ_POD);
    engine->RegisterObjectProperty("Event1", "int foo", asOFFSET(Event1,foo));

    engine->RegisterObjectType("TestServiceType", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectMethod("TestServiceType", "void register(?&in)", asMETHOD(TestService, registerObject), asCALL_THISCALL);
    engine->RegisterGlobalProperty("TestServiceType TestService", this);
  }

  void process(float delta)
  {
    eventBus->queue({Event1::ID, new Event1{42}, nullptr});
    for(asIScriptObject* obj : registered)
    {
      eventBus->queue({Event1::ID, new Event1{7}, obj});
    }
  }

  void registerObject(void* ptr, int typeId)
  {
    if(typeId & asTYPEID_OBJHANDLE)
    {
      void *obj = *reinterpret_cast<void**>(ptr);
      asIScriptObject* o = static_cast<asIScriptObject*>(obj);
      registered.push_back(o);
      o->AddRef();
    }
  }

private:
  asIScriptEngine* engine;
  std::vector<asIScriptObject*> registered;
};

int TestService::Event1::ID = -1;

std::string const INIT_SCRIPT = R"(
  class Entity1
  {
    Entity1()
    {
      print("** I'm born! **\n");
      register(@this);
      TestService.register(@this);
    }

    ~Entity1()
    {
      print("** I'm dying! **\n");
      unregister(@this);
    }
    void onEvent1(Event1 &in e)
    {
      print("foo: " + e.foo + "\n");
    }
  }


  void init() {
    Entity1 entity;
    print("Hello World!\n");
  }
)";

int main(int argc, char** argv)
{
  ew::Engine engine;
  AngelWorksState angelWorksState;
  angelWorksState.registerAndRequireService<TestService>("test");
  angelWorksState.initialize(INIT_SCRIPT);
  engine.addState(0, &angelWorksState);
  engine.setState(0);

  engine.run();
  engine.advance(1.0f);
}
