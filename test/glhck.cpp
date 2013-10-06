#include "GLFW/glfw3.h"
#include "glhck/glhck.h"
#include "ew/engine.h"
#include "ew/interceptor.h"

#include "angelworksstate.h"
#include "angelworksservice.h"
#include "glhckservice.h"
#include "timerservice.h"

#include <boost/filesystem.hpp>
#include <iostream>

std::string const INIT_SCRIPT = R"(
  class Cube
  {
    Cube()
    {
      register(@this);
      @o = GlhckService.createCube(50);

      glhck::Text text(512,512);
      uint font = text.setKakwafont();
      @yay = GlhckService.createTextObject(text, font, 48, "YAYYY!!", glhck::defaultTextureParameters());
      TimerService.setInterval(@this, 0.01);
    }

    ~Cube()
    {
      unregister(@this);
    }

    void onTimerEvent(TimerEvent e)
    {
      o.position = glhck::Vec3(o.position.x + 5, 240, 0);
      o.rotation = glhck::Vec3(0, 0, o.rotation.z + 5);
      yay.position = glhck::Vec3(o.position.x, o.position.y+90, 0);
      yay.material.diffuse = glhck::Color(255, o.rotation.z, o.rotation.z, 255);
    }

    glhck::Object@ o;
    glhck::Object@ yay;
  }

  class Sprite
  {
    Sprite(glhck::Texture@ tex)
    {
      @o = GlhckService.createSprite(tex, 96, 96);
      o.position = glhck::Vec3(400, 120, 0);
    }
    glhck::Object@ o;
  }

  void init() {
    Cube cube;

    glhck::TextureParameters textureParameters = glhck::defaultTextureSpriteParameters();
    glhck::ImportImageParameters imageParameters = glhck::defaultImportImageParameters();
    glhck::Texture tex("img/infantry_1.png", imageParameters, textureParameters);

    Sprite sprite(@tex);
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
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "angelworks-test-glhck", nullptr, nullptr);
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
  angelWorksState.registerAndRequireService<TimerService>("timer");
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
