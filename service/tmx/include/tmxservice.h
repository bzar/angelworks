#ifndef ANGELWORKS_TMX_SERVICE
#define ANGELWORKS_TMX_SERVICE

#include "angelworksservice.h"
#include <string>
#include <unordered_map>
#include "Tmx.h"

class TmxService : public AngelWorksService
{
public:
  void registerToEngine(asIScriptEngine* engine);
  Tmx::Map* parse(std::string const& filename);
  virtual void process(float delta);

private:
  static std::unordered_map<Tmx::Map*, unsigned int> maps;
  static void addMapRef(Tmx::Map* map);
  static void delMapRef(Tmx::Map* map);
};

#endif
