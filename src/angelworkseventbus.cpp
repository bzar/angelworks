#include "angelworkseventbus.h"
#include <iostream>
#include <string>

AngelWorksEventBus::AngelWorksEventBus() :
  ctx(nullptr), events(), eventHandlerMap(), typeObjectMap()
{

}

void AngelWorksEventBus::setContext(asIScriptContext* newCtx)
{
  ctx = newCtx;
}

void AngelWorksEventBus::registerObject(asIScriptObject * const obj)
{
  auto existingTypeIterator = typeObjectMap.find(obj->GetTypeId());
  if(existingTypeIterator != typeObjectMap.end())
  {
    typeObjectMap.insert(existingTypeIterator, std::make_pair(obj->GetTypeId(), obj));
  }
  else
  {
    addObjectTypeEventMapping(obj->GetObjectType());
    typeObjectMap.emplace(std::make_pair(obj->GetTypeId(), obj));
  }
}

void AngelWorksEventBus::unregisterObject(asIScriptObject * const obj)
{
   for(auto iters = typeObjectMap.equal_range(obj->GetTypeId()); iters.first != iters.second; ++iters.first)
   {
     if(iters.first->second == obj)
     {
       typeObjectMap.erase(iters.first);
       break;
     }
   }
}

void AngelWorksEventBus::handle(const AngelWorksEventBus::Event& e)
{
  if(e.target != nullptr)
  {
    processTargetedEvent(e);
  }
  else
  {
    processBroadcastEvent(e);
  }
}

void AngelWorksEventBus::queue(const AngelWorksEventBus::Event &e)
{
  events.push_back(e);
}

void AngelWorksEventBus::process()
{
  for(Event const& e : events)
  {
    handle(e);
    delete e.obj;
  }

  events.clear();
}

void AngelWorksEventBus::addObjectTypeEventMapping(asIObjectType * const objType)
{
  for(asUINT i = 0; i < objType->GetMethodCount(); ++i)
  {
    asIScriptFunction* method = objType->GetMethodByIndex(i);
    std::cout.flush();

    std::string methodName = method->GetName();
    if(methodName.length() > 2
       && methodName.at(0) == 'o'
       && methodName.at(1) == 'n')
    {
      std::string eventName = methodName.substr(2);
      asIObjectType* eventObjectType = ctx->GetEngine()->GetObjectTypeByName(eventName.c_str());
      if(eventObjectType != nullptr)
      {
        eventHandlerMap.insert(std::make_pair(eventObjectType->GetTypeId(), Handler{objType->GetTypeId(), i}));
      }
    }
  }
}

void AngelWorksEventBus::processTargetedEvent(const AngelWorksEventBus::Event& e)
{
  auto handlerRange = eventHandlerMap.equal_range(e.typeId);
  asIScriptFunction* method = nullptr;
  for(auto i = handlerRange.first; i != handlerRange.second; ++i)
  {
    Handler& handler = i->second;
    if(e.target->GetObjectType()->GetTypeId() == handler.objectTypeId)
    {
      method = e.target->GetObjectType()->GetMethodByIndex(handler.methodIndex);
      break;
    }
  }

  if(method != nullptr)
  {
    ctx->Prepare(method);
    if(method->GetParamCount() != 0)
    {
      ctx->SetArgObject(0, e.obj->getObject());
    }
    ctx->SetObject(e.target);
    ctx->Execute();
  }
}

void AngelWorksEventBus::processBroadcastEvent(const AngelWorksEventBus::Event& e)
{
  auto handlerRange = eventHandlerMap.equal_range(e.typeId);
  for(auto i = handlerRange.first; i != handlerRange.second; ++i)
  {
    Handler& handler = i->second;
    asIObjectType* objType = ctx->GetEngine()->GetObjectTypeById(handler.objectTypeId);
    asIScriptFunction* method =objType->GetMethodByIndex(handler.methodIndex);

    auto objRange = typeObjectMap.equal_range(handler.objectTypeId);
    for(auto j = objRange.first; j != objRange.second; ++j)
    {
      ctx->Prepare(method);
      if(method->GetParamCount() != 0)
      {
        ctx->SetArgObject(0, e.obj->getObject());
      }

      asIScriptObject* obj = j->second;
      ctx->SetObject(obj);
      ctx->Execute();
    }
  }
}

