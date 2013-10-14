#include "tmxservice.h"

void TmxService::registerToEngine(asIScriptEngine* engine)
{
  engine->RegisterObjectType("TmxServiceType", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterGlobalProperty("TmxServiceType TmxService", this);
}

void TmxService::load(const std::string& filename)
{

}
