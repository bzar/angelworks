#ifndef ANGELWORKS_STATE
#define ANGELWORKS_STATE

#include "ew/state.h"
#include "ew/customphase.h"
#include "angelscript.h"
#include "angelworkseventbus.h"
//#include "scriptany/scriptany.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

class AngelWorksService;

class AngelWorksState : public ew::State
{
public:
  AngelWorksState();
  ~AngelWorksState();
  void initialize(std::string const& initScript);

  void servicePhase(float delta);
  void eventPhase(float delta);

  template<typename S>
  void registerService(std::string const& name);

  template<typename S>
  void registerAndRequireService(std::string const& name);

  void require(std::string const& name);

  void registerObject(void* ptr, int typeId);
  void unregisterObject(void* ptr, int typeId);

private:
  typedef ew::CustomPhase<AngelWorksState, &AngelWorksState::servicePhase> ServicePhase;
  typedef ew::CustomPhase<AngelWorksState, &AngelWorksState::eventPhase> EventPhase;

  asIScriptEngine* scriptEngine;
  asIScriptContext* scriptContext;

  AngelWorksEventBus eventBus;
  std::unordered_map<std::string, std::function<AngelWorksService*()>> serviceFactories;
  std::unordered_map<std::string, AngelWorksService*> services;
};

template<typename S>
void AngelWorksState::registerService(std::string const& name)
{
  serviceFactories.insert(make_pair(name, [](){ return new S(); }));
}

template<typename S>
void AngelWorksState::registerAndRequireService(std::string const& name)
{
  registerService<S>(name);
  require(name);
}

#endif
