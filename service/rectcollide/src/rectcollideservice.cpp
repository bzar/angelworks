#include "rectcollideservice.h"
#include <algorithm>

int RectCollideService::RectCollideServiceDone::ID = -1;

void RectCollideService::registerToEngine(asIScriptEngine* e)
{
  engine = e;
  RectCollideServiceDone::ID = engine->RegisterObjectType("RectCollideServiceDone", sizeof(RectCollideServiceDone), asOBJ_VALUE | asOBJ_POD);
  //engine->RegisterObjectProperty("TimerEvent", "int timerId", asOFFSET(TimerEvent,timerId));

  engine->RegisterObjectType("CollidableRect", sizeof(CollidableRect), asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectProperty("CollidableRect", "float x", asOFFSET(CollidableRect,x));
  engine->RegisterObjectProperty("CollidableRect", "float y", asOFFSET(CollidableRect,y));
  engine->RegisterObjectProperty("CollidableRect", "float w", asOFFSET(CollidableRect,w));
  engine->RegisterObjectProperty("CollidableRect", "float h", asOFFSET(CollidableRect,h));
  engine->RegisterObjectProperty("CollidableRect", "float vx", asOFFSET(CollidableRect,vx));
  engine->RegisterObjectProperty("CollidableRect", "float vy", asOFFSET(CollidableRect,vy));
  engine->RegisterObjectProperty("CollidableRect", "bool blockable", asOFFSET(CollidableRect,blockable));
  engine->RegisterObjectProperty("CollidableRect", "bool blocking", asOFFSET(CollidableRect,blocking));
  engine->RegisterObjectProperty("CollidableRect", "bool active", asOFFSET(CollidableRect,active));

  engine->RegisterObjectType("RectCollideServiceType", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod("RectCollideServiceType",
                               "CollidableRect@ createRect(?&in, float, float, float, float, float, float, bool, bool, bool)",
                               asMETHOD(RectCollideService, createRect), asCALL_THISCALL);

  //engine->RegisterObjectMethod("RectCollideServiceType", "void setInterval(?&in, float)", asMETHOD(RectCollideService, setInterval), asCALL_THISCALL);
  engine->RegisterGlobalProperty("RectCollideServiceType RectCollideService", this);
}

void RectCollideService::process(float delta)
{
  for(CollidableRect* const rect : rects)
  {
    rect->y += rect->vy;
  }

  for(int i = 0; i < rects.size(); ++i)
  {
    for(int j = i + 1; j < rects.size(); ++j)
    {
      CollidableRect& a = *rects.at(i);
      CollidableRect& b = *rects.at(j);

      if((!a.blocking || !b.blockable) &&
         (!b.blocking || !a.blockable) &&
         !a.active && !b.active)
      {
        continue;
      }

      if(!collide(a, b))
      {
        continue;
      }

      if(a.blockable && b.blocking)
      {
        a.y = a.vy > b.vy ? b.y - b.h : b.y + b.h;
      }

      if(b.blockable && a.blocking)
      {
        b.y = b.vy > a.vy ? a.y - a.h : a.y + a.h;
      }

      if(a.active)
      {
        // TODO: send collision event
      }

      if(b.active)
      {
        // TODO: send collision event
      }
    }
  }

  for(CollidableRect* const rect : rects)
  {
    rect->x += rect->vx;
  }

  for(int i = 0; i < rects.size(); ++i)
  {
    for(int j = i + 1; j < rects.size(); ++j)
    {
      CollidableRect& a = *rects.at(i);
      CollidableRect& b = *rects.at(j);

      if((!a.blocking || !b.blockable) &&
         (!b.blocking || !a.blockable) &&
         !a.active && !b.active)
      {
        continue;
      }

      if(!collide(a, b))
      {
        continue;
      }

      if(a.blockable && b.blocking)
      {
        a.x = a.vx > b.vx ? b.x - b.w : b.x + b.w;
      }

      if(b.blockable && a.blocking)
      {
        b.x = b.vx > a.vx ? a.x - a.w : a.x + a.w;
      }

      if(a.active)
      {
        // TODO: send collision event
      }

      if(b.active)
      {
        // TODO: send collision event
      }
    }
  }

  eventBus->queue({RectCollideServiceDone::ID, new RectCollideServiceDone{}, nullptr});
}

RectCollideService::CollidableRect* RectCollideService::createRect(void* ptr, int typeId, float x, float y, float w, float h,
                                                                   float vx, float vy, bool blocking, bool blockable, bool active)
{
  asIScriptObject* o = nullptr;
  if(typeId != asTYPEID_VOID)
  {
    if(typeId & asTYPEID_OBJHANDLE)
    {
      void *obj = *reinterpret_cast<void**>(ptr);
      o = static_cast<asIScriptObject*>(obj);
    }
    else
    {
      o = static_cast<asIScriptObject*>(ptr);
    }
  }

  CollidableRect* rect = new CollidableRect{x, y, w, h, vx, vy, blocking, blockable, active, o};
  rects.push_back(rect);
  if(o)
  {
    o->AddRef();
  }

  return rect;
}

bool RectCollideService::collide(const RectCollideService::CollidableRect& a,
                                 const RectCollideService::CollidableRect& b)
{
  return collide(a.x, a.y, a.w, a.h, b.x, b.y, b.w, b.h);
}

bool RectCollideService::collide(float x1, float y1, float w1, float h1,
                                 float x2, float y2, float w2, float h2)
{
  return x1 < x2 + w2 && y1 < y2 + h2 &&
         x2 < x1 + w1 && y2 < y1 + h1;
}
