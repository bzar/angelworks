#include "GLFW/glfw3.h"
#include "glhck/glhck.h"
#include "ew/engine.h"
#include "ew/interceptor.h"

#include "angelworksstate.h"
#include "angelworksservice.h"
#include "glhckservice.h"
#include "timerservice.h"
#include "tmxservice.h"

#include <boost/filesystem.hpp>
#include <iostream>

std::string const INIT_SCRIPT = R"(
  array<glhck::Object@> objs;

  void setTextureTransform(glhck::Material@ mat, const tmx::Tileset@ tileset, const int tileId) {
    int tilesPerRow = (tileset.image.width - tileset.margin) / (tileset.tileWidth + tileset.spacing);
    int row = tileId / tilesPerRow;
    int col = tileId - tilesPerRow * row;
    float x = (tileset.margin + col * (tileset.tileWidth + tileset.spacing)) / (tileset.image.width + 0.0);
    float y = (tileset.margin + row * (tileset.tileHeight + tileset.spacing)) / (tileset.image.height + 0.0);
    float w = tileset.tileWidth / (tileset.image.width + 0.0);
    float h = tileset.tileHeight / (tileset.image.height + 0.0);
    mat.textureOffset = glhck::Vec2(x, 1-y);
    mat.textureScale = glhck::Vec2(w, h);
  }

  void init() {
    tmx::Map@ map = TmxService.parse("tmx/test.tmx");

    array<glhck::Texture@> tilesetTextures;

    for(int i = 0; i < map.numTilesets; ++i) {
      const tmx::Tileset@ ts = map.getTileset(i);
      glhck::Texture@ texture = glhck::Texture(map.filepath + ts.image.source, glhck::defaultImportImageParameters(), glhck::defaultTextureSpriteParameters());
      if(@texture != null) {
        tilesetTextures.insertLast(texture);
      } else {
        print("Failed to load tileset texture!\n");
      }
    }

    for(int i = 0; i < map.numLayers; ++i) {
      for(int y = 0; y < map.getLayer(i).height; ++y) {
        for(int x = 0; x < map.getLayer(i).width; ++x) {
          if(map.getLayer(i).getTile(x, y).tilesetId >= 0) {
            glhck::Texture@ texture = tilesetTextures[map.getLayer(i).getTile(x, y).tilesetId];
            glhck::Object@ tile = GlhckService.createSprite(texture, 16, 16);
            tile.position = glhck::Vec3(x*16 + 8, y*16 + 8, 0);
            setTextureTransform(tile.material, map.getTileset(map.getLayer(i).getTile(x, y).tilesetId), map.getLayer(i).getTile(x, y).id);
            objs.insertLast(tile);
          }
        }
      }
    }
    print("Successfully loaded map " + map.filename + "\n");
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
  angelWorksState.registerAndRequireService<TmxService>("tmx");

  angelWorksState.initialize(INIT_SCRIPT);
  engine.addState(0, &angelWorksState);
  engine.setState(0);

  engine.run();

  double t0 = glfwGetTime();
  while(engine.isRunning() && glfwGetTime() - t0 < 3)
  {
    engine.advance(1.0f/60.0f);
  }
}
