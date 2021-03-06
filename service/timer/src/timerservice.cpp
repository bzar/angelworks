#include "timerservice.h"
#include <algorithm>

int TimerService::TimerEvent::ID = -1;

TimerService::~TimerService()
{
  for(Timer& timer : timers)
  {
    timer.objWeakRefFlag->Release();
  }
  timers.clear();
}

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
  addTimer(ptr, typeId, dt, false);
}

int TimerService::setInterval(void* ptr, int typeId, float dt)
{
  addTimer(ptr, typeId, dt, true);
}

void TimerService::process(float delta)
{
  time += delta;
  bool expiredTimers = false;
  auto expired = [&](Timer& timer) {
    return time - delta <= timer.start + timer.interval && time > timer.start + timer.interval;
  };

  for(Timer& timer : timers)
  {
    bool remove = false;
    timer.objWeakRefFlag->Lock();
    if(timer.objWeakRefFlag->Get())
    {
      remove = true;
    }
    else if(expired(timer))
    {
      eventBus->queue(TimerEvent::ID, new TimerEvent{timer.id}, timer.obj);
      if(timer.repeat)
      {
        timer.start = time;
      }
      else
      {
        expiredTimers = true;
        remove = true;
      }
    }

    if(remove)
    {
      timer.objWeakRefFlag->Unlock();
      timer.objWeakRefFlag->Release();
      timer.objWeakRefFlag = nullptr;
      timer.obj = nullptr;
    }
    else
    {
      timer.objWeakRefFlag->Unlock();
    }
  }

  auto removeIter = std::remove_if(timers.begin(), timers.end(), [&expired](Timer& t) {
    return t.objWeakRefFlag == nullptr || t.obj == nullptr || (!t.repeat && expired(t));
  });

  timers.erase(removeIter, timers.end());
}

int TimerService::addTimer(void* ptr, int typeId, float dt, bool repeat)
{
  if(typeId & asTYPEID_OBJHANDLE)
  {
    void *obj = *reinterpret_cast<void**>(ptr);
    asIScriptObject* o = static_cast<asIScriptObject*>(obj);
    asILockableSharedBool* weakRefFlag = engine->GetWeakRefFlagOfScriptObject(o, o->GetObjectType());
    Timer timer{o, weakRefFlag, ++idCounter, time, dt, repeat};
    timers.push_back(timer);
    timer.objWeakRefFlag->AddRef();
  }
}
