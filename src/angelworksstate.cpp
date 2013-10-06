#include "angelworksstate.h"
#include "scriptstdstring/scriptstdstring.h"
#include "scriptarray/scriptarray.h"
#include <iostream>
#include <sstream>
#include "ew/engine.h"
#include "angelworksservice.h"
#include "ew/renderphase.h"

namespace
{
  void messageCallback(const asSMessageInfo *msg, void *param)
  {
    const char *type = "ERR ";
    if( msg->type == asMSGTYPE_WARNING )
      type = "WARN";
    else if( msg->type == asMSGTYPE_INFORMATION )
      type = "INFO";
    std::ostringstream oss;

    oss << msg->section << "(" << msg->row << ", " << msg->col << ") : " << type << " : " << msg->message << std::endl;
    std::cout << oss.str();
  }

  void print(std::string &msg)
  {
    std::cout << msg.data();
  }
  void genericTest(asIScriptGeneric* gen)
  {
    std::string& str = *static_cast<std::string*>(gen->GetArgObject(0));

    if(str == "foo")
    {
      gen->SetReturnObject(new std::string("bar"));
    }
    else
    {
      gen->SetReturnDWord(42);
    }
  }
}

AngelWorksState::AngelWorksState() : ew::State(),
  scriptEngine(asCreateScriptEngine(ANGELSCRIPT_VERSION)), scriptContext(nullptr),
  eventBus(), serviceFactories(), services()
{
  phases = {new ServicePhase(this, this), new EventPhase(this, this), new ew::RenderPhase(this)};

  scriptEngine->SetMessageCallback(asFUNCTION(messageCallback), 0, asCALL_CDECL);
  RegisterStdString(scriptEngine);
  RegisterScriptArray(scriptEngine, true);
  scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
  scriptEngine->RegisterGlobalFunction("void require(const string &in)", asMETHOD(AngelWorksState, require), asCALL_THISCALL_ASGLOBAL, this);
  scriptEngine->RegisterGlobalFunction("void register(?&in)", asMETHOD(AngelWorksState, registerObject), asCALL_THISCALL_ASGLOBAL, this);
  scriptEngine->RegisterGlobalFunction("void unregister(?&in)", asMETHOD(AngelWorksState, unregisterObject), asCALL_THISCALL_ASGLOBAL, this);
  scriptEngine->SetUserData(this);
}

AngelWorksState::~AngelWorksState()
{
  for(auto& servicePair : services)
  {
    delete servicePair.second;
  }
  services.clear();
  scriptContext->Release();
  scriptEngine->Release();
}

void AngelWorksState::initialize(const std::string& initScript)
{
  asIScriptModule *mod = scriptEngine->GetModule("module", asGM_ALWAYS_CREATE);
  mod->AddScriptSection("init", initScript.data());
  mod->Build();

  if(scriptContext != nullptr)
  {
    scriptContext->Release();
  }

  scriptContext = scriptEngine->CreateContext();
  eventBus.setContext(scriptContext);
  asIScriptFunction* initFunc = mod->GetFunctionByName("init");
  scriptContext->Prepare(initFunc);
  scriptContext->Execute();
}

void AngelWorksState::servicePhase(float delta)
{
  for(auto& servicePair : services)
  {
    servicePair.second->process(delta);
  }
}

void AngelWorksState::eventPhase(float delta)
{
  eventBus.process();
}

void AngelWorksState::require(const std::string& name)
{
  auto existingServiceIter = services.find(name);
  if(existingServiceIter == services.end())
  {
    auto serviceFactoryIter = serviceFactories.find(name);
    if(serviceFactoryIter != serviceFactories.end())
    {
      AngelWorksService* service = serviceFactoryIter->second();
      service->registerToEngine(scriptEngine);
      service->setEventBus(&eventBus);
      service->setState(this);
      services.insert(make_pair(name, service));
    }
  }
}

void AngelWorksState::registerObject(void* ptr, int typeId)
{
  if(typeId & asTYPEID_OBJHANDLE)
  {
    void *obj = *reinterpret_cast<void**>(ptr);
    asIScriptObject* o = static_cast<asIScriptObject*>(obj);
    eventBus.registerObject(o);
  }
}

void AngelWorksState::unregisterObject(void* ptr, int typeId)
{
  if(typeId & asTYPEID_OBJHANDLE)
  {
    void *obj = *reinterpret_cast<void**>(ptr);
    asIScriptObject* o = static_cast<asIScriptObject*>(obj);
    eventBus.unregisterObject(o);
  }
}
