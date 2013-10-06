#include "glhckservice.h"
#include "anglhck.h"

void GlhckService::registerToEngine(asIScriptEngine* e)
{
  engine = e;
  anglhck::registerToEngine(engine);

  engine->RegisterObjectType("GlhckServiceType", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod("GlhckServiceType", "glhck::Object@ createCube(const float)", asMETHOD(GlhckService, createCube), asCALL_THISCALL);
  engine->RegisterGlobalProperty("GlhckServiceType GlhckService", this);
}

glhckObject* GlhckService::createCube(const float size)
{
  glhckObject* cube = glhckCubeNew(size);
  objects.push_back(cube);
  glhckObjectRef(cube);
  return cube;
}

void GlhckService::process(float delta)
{

}

void GlhckService::setState(ew::State* value)
{
  if(state != nullptr)
  {
    state->entities.remove<ew::Renderable>(this);
  }
  AngelWorksService::setState(value);
  if(state != nullptr)
  {
    state->entities.insert<ew::Renderable>(this);
  }

}

void GlhckService::render()
{
  for(glhckObject* o : objects)
  {
    glhckObjectDraw(o);
  }
}

