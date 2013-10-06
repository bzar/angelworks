#include "timerservice.h"

int TimerService::TimerEvent::ID = -1;

void TimerService::registerToEngine(asIScriptEngine* e)
{
  engine = e;
  TimerEvent::ID = engine->RegisterObjectType("TimerEvent", sizeof(TimerEvent), asOBJ_VALUE | asOBJ_POD);
  engine->RegisterObjectProperty("TimerEvent", "int timerId", asOFFSET(TimerEvent,timerId));

  engine->RegisterObjectType("TimerServiceType", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod("TimerServiceType", "void setTimeout(?&in, float)", asMETHOD(TimerService, setTimeout), asCALL_THISCALL);
  engine->RegisterObjectMethod("TimerServiceType", "void setInterval(?&in, float)", asMETHOD(TimerService, setInterval), asCALL_THISCALL);
  engine->RegisterGlobalProperty("TimerServiceType TimerService", this);
}

int TimerService::setTimeout(void* ptr, int typeId, float dt)
{
  if(typeId & asTYPEID_OBJHANDLE)
  {
    void *obj = *reinterpret_cast<void**>(ptr);
    asIScriptObject* o = static_cast<asIScriptObject*>(obj);
    Timer timer{o, ++idCounter, time, dt, false};
    timers.push_back(timer);
    o->AddRef();
  }
}

int TimerService::setInterval(void* ptr, int typeId, float dt)
{
  if(typeId & asTYPEID_OBJHANDLE)
  {
    void *obj = *reinterpret_cast<void**>(ptr);
    asIScriptObject* o = static_cast<asIScriptObject*>(obj);
    Timer timer{o, ++idCounter, time, dt, true};
    timers.push_back(timer);
    o->AddRef();
  }
}

void TimerService::process(float delta)
{
  time += delta;

  for(Timer& timer : timers)
  {
    if(time - delta <= timer.start + timer.interval && time > timer.start + timer.interval)
    {
      eventBus->queue({TimerEvent::ID, new TimerEvent{timer.id}, timer.obj});
      if(timer.repeat)
      {
        timer.start = time;
      }
    }
  }
}
