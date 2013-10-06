#include "glhckservice.h"
#include "anglhck.h"

void GlhckService::registerToEngine(asIScriptEngine* e)
{
  engine = e;
  anglhck::registerToEngine(engine);

  engine->RegisterObjectType("GlhckServiceType", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod("GlhckServiceType", "glhck::Object@ createCube(const float)", asMETHOD(GlhckService, createCube), asCALL_THISCALL);
  engine->RegisterObjectMethod("GlhckServiceType", "glhck::Object@ createSprite(glhck::Texture@ texture, const float width, const float height)", asMETHOD(GlhckService, createSprite), asCALL_THISCALL);
  engine->RegisterObjectMethod("GlhckServiceType", "glhck::Object@ createModel(const string &in, const float, const glhck::ImportModelParameters &in)", asMETHOD(GlhckService, createModel), asCALL_THISCALL);
  engine->RegisterObjectMethod("GlhckServiceType", "glhck::Object@ createTextObject(glhck::Text@, const uint8, const float, const string &in, const glhck::TextureParameters &in)", asMETHOD(GlhckService, createTextObject), asCALL_THISCALL);

  engine->RegisterGlobalProperty("GlhckServiceType GlhckService", this);
}

glhckObject* GlhckService::createCube(const float size)
{
  glhckObject* cube = glhckCubeNew(size);
  objects.push_back(cube);
  glhckObjectRef(cube);
  return cube;
}

glhckObject* GlhckService::createSprite(glhckTexture* texture, float width, float height)
{
  glhckObject* sprite = glhckSpriteNew(texture, width, height);
  objects.push_back(sprite);
  glhckObjectRef(sprite);
  return sprite;
}

glhckObject* GlhckService::createModel(const std::string& file, float size, const glhckImportModelParameters* importParams)
{
  glhckObject* model = glhckModelNew(file.data(), size, importParams);
  objects.push_back(model);
  glhckObjectRef(model);
  return model;
}

glhckObject* GlhckService::createTextObject(glhckText* text, const unsigned int font, const float size, const std::string& str, const glhckTextureParameters& parameters)
{
  glhckObject* textObject = glhckTextPlane(text, font, size, str.data(), &parameters);
  objects.push_back(textObject);
  glhckObjectRef(textObject);
  return textObject;
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

