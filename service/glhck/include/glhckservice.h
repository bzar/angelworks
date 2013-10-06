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
  glhckObject* createSprite(glhckTexture* texture, float width, float height);
  glhckObject* createModel(std::string const& file, float size, const glhckImportModelParameters *importParams);
  glhckObject* createTextObject(glhckText *text, unsigned int const font, float const size, std::string const& str, const glhckTextureParameters &parameters);
  void process(float delta);

private:
  asIScriptEngine* engine;
  std::vector<glhckObject*> objects;
};

#endif
