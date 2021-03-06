#include "GLFW/glfw3.h"
#include "glhck/glhck.h"
#include "ew/engine.h"
#include "ew/interceptor.h"

#include "angelworksstate.h"
#include "angelworksservice.h"
#include "glhckservice.h"
#include "timerservice.h"
#include "rectcollideservice.h"

#include <boost/filesystem.hpp>
#include <iostream>

std::string const INIT_SCRIPT = R"(
  class Sprite
  {
    Sprite(glhck::Texture@ tex)
    {
      register(@this);
      @o = GlhckService.createSprite(tex, 96, 96);
      @rect = RectCollideService.createRect(@this, 400, 120, 96, 96, 0, 25, false, true, true);
      o.position = glhck::Vec3(rect.x + rect.w/2, rect.y + rect.h/2, 0);
    }

    ~Sprite()
    {
      unregister(@this);
    }

    void onRectCollision(RectCollision@ e)
    {
      if(@e.b.object == null)
      {
        rect.vy = -rect.vy;
      }
      else
      {
        Sprite@ sprite = cast<Sprite>(e.b.object);
        if(@sprite != null)
        {
          o.material.diffuse = glhck::Color(255, o.position.y, o.position.y, 255);
        }
      }

    }
    void onRectCollideServiceDone()
    {
      o.position = glhck::Vec3(rect.x + rect.w/2, rect.y + rect.h/2, 0);
    }

    glhck::Object@ o;
    CollidableRect@ rect;
  }

  void init() {
    glhck::TextureParameters textureParameters = glhck::defaultTextureSpriteParameters();
    glhck::ImportImageParameters imageParameters = glhck::defaultImportImageParameters();
    glhck::Texture tex("img/infantry_1.png", imageParameters, textureParameters);

    Sprite sprite(@tex);
    Sprite sprite2(@tex);
    sprite2.rect.vy = -15;
    RectCollideService.createRect(null, 400, 480, 96, 96, 0, 0, true, false, false);
    RectCollideService.createRect(null, 400, -96, 96, 96, 0, 0, true, false, false);
  }
)";

void windowCloseCallback(GLFWwindow* window);
void gameloop(GLFWwindow* window);

int const WIDTH = 800;
int const HEIGHT = 480;

struct GLFWContext
{
  GLFWwindow* window;
};

class GlhckGLFWInterceptor : public ew::Interceptor<ew::Engine>
{
  void before(ew::Engine *t)
  {
    glfwPollEvents();
    glhckRenderClear(GLHCK_DEPTH_BUFFER | GLHCK_COLOR_BUFFER);
  }

  void after(ew::Engine *engine)
  {
    GLFWContext* ctx = engine->singletons.get<GLFWContext>();
    glhckRender();
    glfwSwapBuffers(ctx->window);
  }
};

int main(int argc, char** argv)
{
  std::cout << argv[0] << std::endl;
  chdir(boost::filesystem::path(argv[0]).remove_leaf().c_str());

  if (!glfwInit())
    return EXIT_FAILURE;

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "angelworks-test-rectcollide", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if(!window)
  {
    return EXIT_FAILURE;
  }

  glfwSwapInterval(1);
  glfwSetWindowCloseCallback(window, windowCloseCallback);

  if(!glhckContextCreate(argc, argv))
  {
    std::cerr << "GLHCK initialization error" << std::endl;
    return EXIT_FAILURE;
  }

  glhckLogColor(0);
  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_AUTO))
  {
    std::cerr << "GLHCK display create error" << std::endl;
    return EXIT_FAILURE;
  }


  gameloop(window);


  glhckContextTerminate();
  glfwTerminate();
}

void windowCloseCallback(GLFWwindow* window)
{
  ew::Engine* engine = static_cast<ew::Engine*>(glfwGetWindowUserPointer(window));
  engine->quit();
}

void gameloop(GLFWwindow* window)
{
  ew::Engine engine;
  glfwSetWindowUserPointer(window, &engine);

  GLFWContext glfwContext = {window};
  engine.singletons.set<GLFWContext>(&glfwContext);

  GlhckGLFWInterceptor glhckGLFWInterceptor;
  engine.addInterceptor(&glhckGLFWInterceptor);

  AngelWorksState angelWorksState;
  angelWorksState.registerAndRequireService<GlhckService>("glhck");
  angelWorksState.registerAndRequireService<RectCollideService>("rectcollide");
  angelWorksState.initialize(INIT_SCRIPT);
  engine.addState(0, &angelWorksState);
  engine.setState(0);

  engine.run();

  double t0 = glfwGetTime();
  while(engine.isRunning() && glfwGetTime() - t0 < 4)
  {
    engine.advance(1.0f/60.0f);
  }
}
