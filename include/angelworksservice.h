#ifndef ANGELWORKSSERVICE_H
#define ANGELWORKSSERVICE_H

#include <string>
#include "angelworkseventbus.h"
#include "ew/state.h"

class asIScriptEngine;

class AngelWorksService
{
public:
  virtual ~AngelWorksService() {}
  virtual void registerToEngine(asIScriptEngine* engine) = 0;
  virtual void process(float delta) = 0;
  virtual void setEventBus(AngelWorksEventBus* value) { eventBus = value; }
  virtual void setState(ew::State* value) { state = value; }
protected:
  AngelWorksEventBus* eventBus;
  ew::State* state;

};

#endif // ANGELWORKSSERVICE_H
