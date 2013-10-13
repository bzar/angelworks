#ifndef ANGELWORKS_EVENTBUS_H
#define ANGELWORKS_EVENTBUS_H

#include "angelscript.h"
#include <vector>
#include <unordered_map>
#include "angelworksevent.h"

class AngelWorksEventBus
{
public:
  struct Event
  {
    typedef int ID;
    ID const typeId;
    AngelWorksEvent* const obj;
    asIScriptObject* target;
  };

  AngelWorksEventBus();

  void setContext(asIScriptContext* newCtx);
  void registerObject(asIScriptObject* const obj);
  void unregisterObject(asIScriptObject* const obj);
  void handle(Event const& e);
  template<typename T> void queue(Event::ID const typeId, T* obj, asIScriptObject* target);
  void queue(Event const& e);
  void process();

private:
  struct Handler
  {
    int objectTypeId;
    asUINT methodIndex;
  };

  void addObjectTypeEventMapping(asIObjectType* const objType);
  void processTargetedEvent(Event const& e);
  void processBroadcastEvent(Event const& e);

  asIScriptContext* ctx;
  std::vector<Event> events;

  std::unordered_multimap<Event::ID, Handler> eventHandlerMap;
  std::unordered_multimap<int, asIScriptObject*> typeObjectMap;

};

template<typename T>
void AngelWorksEventBus::queue(Event::ID const typeId, T* obj, asIScriptObject* target)
{
  AngelWorksEventWrapper<T>* wrapper = new AngelWorksEventWrapper<T>(obj);
  queue({typeId, wrapper, target});
}

#endif
