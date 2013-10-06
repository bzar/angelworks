#ifndef ANGELWORKS_TIMER_SERVICE
#define ANGELWORKS_TIMER_SERVICE

#include "angelworksservice.h"

class TimerService : public AngelWorksService
{
public:
  struct TimerEvent
  {
    static int ID;
    int timerId;
  };

  void registerToEngine(asIScriptEngine* e);
  int setTimeout(void* ptr, int typeId, float dt);
  int setInterval(void* ptr, int typeId, float dt);
  void process(float delta);

private:
  struct Timer
  {
    asIScriptObject* obj;
    int id;
    float start;
    float interval;
    bool repeat;
  };

  asIScriptEngine* engine;
  std::vector<Timer> timers;
  int idCounter = 0;
  float time;
};

#endif
