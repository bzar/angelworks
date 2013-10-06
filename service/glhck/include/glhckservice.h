#ifndef ANGELWORKS_GLHCK_SERVICE
#define ANGELWORKS_GLHCK_SERVICE

#include "angelworksservice.h"
#include "glhck/glhck.h"
#include "ew/renderable.h"
#include "ew/state.h"
#include "ew/rolemanager.h"

class GlhckService : public AngelWorksService, public ew::Renderable
{
public:
  void setState(ew::State *value);
  void render();
  void registerToEngine(asIScriptEngine* e);
  glhckObject* createCube(float const size);
  void process(float delta);

private:
  asIScriptEngine* engine;
  std::vector<glhckObject*> objects;
};

#endif
