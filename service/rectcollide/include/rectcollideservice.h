#ifndef ANGELWORKS_RECTCOLLIDE_SERVICE
#define ANGELWORKS_RECTCOLLIDE_SERVICE

#include "angelworksservice.h"
#include "scripthandle/scripthandle.h"
#include <vector>

class RectCollideService : public AngelWorksService
{
public:
  struct CollidableRect
  {
    float x;
    float y;
    float w;
    float h;
    float vx;
    float vy;
    bool blocking;
    bool blockable;
    bool active;
    asIScriptObject* obj;
    CScriptHandle objHandle;
  };

  struct RectCollideServiceDone
  {
    static int ID;
  };

  enum CollisionDirection { UP, DOWN, LEFT, RIGHT };

  struct RectCollision
  {
    static int ID;
    CollidableRect a;
    CollidableRect b;
    CollisionDirection direction;
  };

  ~RectCollideService();
  void registerToEngine(asIScriptEngine* e);
  void process(float delta);
  CollidableRect* createRect(void* ptr, int typeId, float x, float y, float w, float h, float vx, float vy, bool blocking, bool blockable, bool active);

private:
  bool collide(CollidableRect const& a, CollidableRect const& b);
  bool collide(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
  asIScriptEngine* engine;
  std::vector<CollidableRect*> rects;
};

#endif
