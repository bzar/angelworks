#ifndef ANGELWORKS_TMX_SERVICE
#define ANGELWORKS_TMX_SERVICE

#include "angelworksservice.h"
#include <string>

class TmxService : public AngelWorksService
{
public:
  void registerToEngine(asIScriptEngine* engine);
  void load(std::string const& filename);

private:
};

#endif
