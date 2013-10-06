#ifndef ANGELWORKSSERVICE_H
#define ANGELWORKSSERVICE_H

#include <string>
#include "angelworkseventbus.h"
#include "scriptany/scriptany.h"

class asIScriptEngine;

class AngelWorksService
{
public:
  virtual ~AngelWorksService() {}
  virtual void registerToEngine(asIScriptEngine* engine) = 0;
  virtual void process(float delta) = 0;
  void setEventBus(AngelWorksEventBus* value) { eventBus = value; }
protected:
  AngelWorksEventBus* eventBus;
};

#endif // ANGELWORKSSERVICE_H
