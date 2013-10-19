#include "tmxservice.h"

std::unordered_map<Tmx::Map*, unsigned int> TmxService::maps = std::unordered_map<Tmx::Map*, unsigned int>();

void TmxService::registerToEngine(asIScriptEngine* engine)
{
  std::string const previousNameSpace = engine->GetDefaultNamespace();
  engine->SetDefaultNamespace("tmx");

  engine->RegisterObjectType("Map", 0, asOBJ_REF);
  engine->RegisterObjectType("Tileset", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("Tile", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("Image", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("Layer", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("Object", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("ObjectGroup", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("Ellipse", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("Point", sizeof(Tmx::Point), asOBJ_VALUE | asOBJ_POD);
  engine->RegisterObjectType("Polygon", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("Polyline", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("PropertySet", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType("ImageLayer", 0, asOBJ_REF | asOBJ_NOCOUNT);

  engine->RegisterEnum("MapOrientation");
  engine->RegisterEnumValue("MapOrientation", "ORTHOGONAL", 0);
  engine->RegisterEnumValue("MapOrientation", "ISOMETRIC", 1);
  engine->RegisterEnumValue("MapOrientation", "STAGGERED", 2);

  engine->RegisterObjectBehaviour("Map", asBEHAVE_ADDREF, "void f()", asFUNCTION(TmxService::addMapRef), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectBehaviour("Map", asBEHAVE_RELEASE, "void f()", asFUNCTION(TmxService::delMapRef), asCALL_CDECL_OBJFIRST);

  engine->RegisterObjectMethod("Map", "const string& get_filename()", asMETHOD(Tmx::Map, GetFilename), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const string& get_filepath()", asMETHOD(Tmx::Map, GetFilepath), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "float get_version()", asMETHOD(Tmx::Map, GetVersion), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "MapOrientation get_orientation()", asMETHOD(Tmx::Map, GetOrientation), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_width()", asMETHOD(Tmx::Map, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_height()", asMETHOD(Tmx::Map, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_tileWidth()", asMETHOD(Tmx::Map, GetTileWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_tileHeight()", asMETHOD(Tmx::Map, GetTileHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const Layer@ getLayer(int)", asMETHOD(Tmx::Map, GetLayer), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numLayers()", asMETHOD(Tmx::Map, GetNumLayers), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const ObjectGroup@ getObjectGroup(int)", asMETHOD(Tmx::Map, GetObjectGroup), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numObjectGroups()", asMETHOD(Tmx::Map, GetNumObjectGroups), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const ImageLayer@ getImageLayer(int)", asMETHOD(Tmx::Map, GetImageLayer), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numImageLayers()", asMETHOD(Tmx::Map, GetNumImageLayers), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int findTilesetIndex(int)", asMETHOD(Tmx::Map, FindTilesetIndex), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const Tileset@ findTileset(int)", asMETHOD(Tmx::Map, FindTileset), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const Tileset@ getTileset(int)", asMETHOD(Tmx::Map, GetTileset), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numTilesets()", asMETHOD(Tmx::Map, GetNumTilesets), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "bool get_error()", asMETHOD(Tmx::Map, HasError), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const string& get_errorText()", asMETHOD(Tmx::Map, GetErrorText), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_errorCode()", asMETHOD(Tmx::Map, GetErrorCode), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const PropertySet@ get_properties()", asMETHOD(Tmx::Map, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("Tileset", "int get_firstGid()", asMETHOD(Tmx::Tileset, GetFirstGid), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const string& get_name()", asMETHOD(Tmx::Tileset, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_tileWidth()", asMETHOD(Tmx::Tileset, GetTileWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_tileHeight()", asMETHOD(Tmx::Tileset, GetTileHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_margin()", asMETHOD(Tmx::Tileset, GetMargin), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_spacing()", asMETHOD(Tmx::Tileset, GetSpacing), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const Image@ get_image()", asMETHOD(Tmx::Tileset, GetImage), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const Tile@ getTile(int)", asMETHOD(Tmx::Tileset, GetTile), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const PropertySet@ get_properties()", asMETHOD(Tmx::Tileset, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("Tile", "int get_id()", asMETHOD(Tmx::Tile, GetId), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tile", "const PropertySet@ get_properties()", asMETHOD(Tmx::Tile, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("Image", "int get_width()", asMETHOD(Tmx::Image, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Image", "int get_height()", asMETHOD(Tmx::Image, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Image", "const string& get_source()", asMETHOD(Tmx::Image, GetSource), asCALL_THISCALL);
  engine->RegisterObjectMethod("Image", "const string& get_transparentColor()", asMETHOD(Tmx::Image, GetTransparentColor), asCALL_THISCALL);

  engine->RegisterObjectMethod("Layer", "const string& get_name()", asMETHOD(Tmx::Layer, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "int get_width()", asMETHOD(Tmx::Layer, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "int get_height()", asMETHOD(Tmx::Layer, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool get_visible()", asMETHOD(Tmx::Layer, IsVisible), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "const PropertySet@ get_properties()", asMETHOD(Tmx::Layer, GetProperties), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool getTileId(int, int)", asMETHOD(Tmx::Layer, GetTileId), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool getTileTilesetIndex(int, int)", asMETHOD(Tmx::Layer, GetTileTilesetIndex), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool isTileFlippedHorizontally(int, int)", asMETHOD(Tmx::Layer, IsTileFlippedHorizontally), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool isTileFlippedVertically(int, int)", asMETHOD(Tmx::Layer, IsTileFlippedVertically), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool isTileFlippedDiagonally(int, int)", asMETHOD(Tmx::Layer, IsTileFlippedDiagonally), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "const Tile@ getTile(int, int)", asMETHOD(Tmx::Layer, GetTile), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "int get_zOrder()", asMETHOD(Tmx::Layer, GetZOrder), asCALL_THISCALL);

  engine->RegisterObjectMethod("Object", "const string& get_name()", asMETHOD(Tmx::Object, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const string& get_type()", asMETHOD(Tmx::Object, GetType), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_x()", asMETHOD(Tmx::Object, GetX), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_y()", asMETHOD(Tmx::Object, GetY), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_width()", asMETHOD(Tmx::Object, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_height()", asMETHOD(Tmx::Object, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_gid()", asMETHOD(Tmx::Object, GetGid), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const Ellipse@ get_ellipse()", asMETHOD(Tmx::Object, GetEllipse), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const Polygon@ get_polygon()", asMETHOD(Tmx::Object, GetPolygon), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const Polyline@ get_polyline()", asMETHOD(Tmx::Object, GetPolyline), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const PropertySet@ get_properties()", asMETHOD(Tmx::Object, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("ObjectGroup", "const string& get_name()", asMETHOD(Tmx::ObjectGroup, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_width()", asMETHOD(Tmx::ObjectGroup, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_height()", asMETHOD(Tmx::ObjectGroup, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "const Object@ getObject(int)", asMETHOD(Tmx::ObjectGroup, GetObject), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_numObjects()", asMETHOD(Tmx::ObjectGroup, GetNumObjects), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_visibility()", asMETHOD(Tmx::ObjectGroup, GetVisibility), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "const PropertySet@ get_properties()", asMETHOD(Tmx::ObjectGroup, GetProperties), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_zOrder()", asMETHOD(Tmx::ObjectGroup, GetZOrder), asCALL_THISCALL);

  engine->RegisterObjectMethod("Ellipse", "int get_centerX()", asMETHOD(Tmx::Ellipse, GetCenterX), asCALL_THISCALL);
  engine->RegisterObjectMethod("Ellipse", "int get_centerY()", asMETHOD(Tmx::Ellipse, GetCenterY), asCALL_THISCALL);
  engine->RegisterObjectMethod("Ellipse", "int get_radiusX()", asMETHOD(Tmx::Ellipse, GetRadiusX), asCALL_THISCALL);
  engine->RegisterObjectMethod("Ellipse", "int get_radiusY()", asMETHOD(Tmx::Ellipse, GetRadiusY), asCALL_THISCALL);

  engine->RegisterObjectProperty("Point", "float x", asOFFSET(Tmx::Point, x));
  engine->RegisterObjectProperty("Point", "float y", asOFFSET(Tmx::Point, y));

  engine->RegisterObjectMethod("Polygon", "int get_numPoints()", asMETHOD(Tmx::Polygon, GetNumPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod("Polygon", "const Point& getPoint(int)", asMETHOD(Tmx::Polygon, GetPoint), asCALL_THISCALL);

  engine->RegisterObjectMethod("Polyline", "int get_numPoints()", asMETHOD(Tmx::Polyline, GetNumPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod("Polyline", "const Point& getPoint(int)", asMETHOD(Tmx::Polyline, GetPoint), asCALL_THISCALL);

  engine->RegisterObjectMethod("PropertySet", "int getNumericProperty(const string &in)", asMETHOD(Tmx::PropertySet, GetNumericProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "float getFloatProperty(const string &in)", asMETHOD(Tmx::PropertySet, GetFloatProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "const string getLiteralProperty(const string &in)", asMETHOD(Tmx::PropertySet, GetLiteralProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "int get_size()", asMETHOD(Tmx::PropertySet, GetSize), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "bool hasProperty(const string &in)", asMETHOD(Tmx::PropertySet, HasProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "int get_empty()", asMETHOD(Tmx::PropertySet, Empty), asCALL_THISCALL);

  engine->RegisterObjectMethod("ImageLayer", "const string& get_name()", asMETHOD(Tmx::ImageLayer, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "int get_width()", asMETHOD(Tmx::ImageLayer, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "int get_height()", asMETHOD(Tmx::ImageLayer, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "bool get_visible()", asMETHOD(Tmx::ImageLayer, IsVisible), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "const Image@ getImage()", asMETHOD(Tmx::ImageLayer, GetImage), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "const PropertySet@ get_properties()", asMETHOD(Tmx::ImageLayer, GetProperties), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "int get_zOrder()", asMETHOD(Tmx::ImageLayer, GetZOrder), asCALL_THISCALL);

  engine->SetDefaultNamespace(previousNameSpace.data());

  engine->RegisterObjectType("TmxServiceType", 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod("TmxServiceType", "tmx::Map@ parse(const string &in)", asMETHOD(TmxService, parse), asCALL_THISCALL);
  engine->RegisterGlobalProperty("TmxServiceType TmxService", this);
}

Tmx::Map* TmxService::parse(const std::string& filename)
{
  Tmx::Map* map = new Tmx::Map();
  map->ParseFile(filename);
  addMapRef(map);
  return map;
}

void TmxService::process(float delta)
{

}

void TmxService::addMapRef(Tmx::Map* map)
{
  auto mapIter = maps.find(map);
  if(mapIter != maps.end())
  {
    mapIter->second += 1;
  }
  else
  {
    maps.insert(std::make_pair(map, 1));
  }
}

void TmxService::delMapRef(Tmx::Map* map)
{
  auto mapIter = maps.find(map);
  if(mapIter != maps.end())
  {
    mapIter->second -= 1;

    if(mapIter->second == 0)
    {
      delete map;
      maps.erase(mapIter);
    }
  }
}
