#include "tmxservice.h"
#include "scriptarray/scriptarray.h"
#include <iostream>

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
  engine->RegisterObjectType("MapTile", sizeof(Tmx::MapTile), asOBJ_VALUE | asOBJ_POD);
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

  engine->RegisterObjectMethod("Map", "const string& get_filename() const", asMETHOD(Tmx::Map, GetFilename), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const string& get_filepath() const", asMETHOD(Tmx::Map, GetFilepath), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "float get_version() const", asMETHOD(Tmx::Map, GetVersion), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "MapOrientation get_orientation() const", asMETHOD(Tmx::Map, GetOrientation), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_width() const", asMETHOD(Tmx::Map, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_height() const", asMETHOD(Tmx::Map, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_tileWidth() const", asMETHOD(Tmx::Map, GetTileWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_tileHeight() const", asMETHOD(Tmx::Map, GetTileHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const Layer@ getLayer(int) const", asMETHOD(Tmx::Map, GetLayer), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numLayers() const", asMETHOD(Tmx::Map, GetNumLayers), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const ObjectGroup@ getObjectGroup(int) const", asMETHOD(Tmx::Map, GetObjectGroup), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numObjectGroups() const", asMETHOD(Tmx::Map, GetNumObjectGroups), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const ImageLayer@ getImageLayer(int) const", asMETHOD(Tmx::Map, GetImageLayer), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numImageLayers() const", asMETHOD(Tmx::Map, GetNumImageLayers), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int findTilesetIndex(int)", asMETHOD(Tmx::Map, FindTilesetIndex), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const Tileset@ findTileset(int)", asMETHOD(Tmx::Map, FindTileset), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const Tileset@ getTileset(int)", asMETHOD(Tmx::Map, GetTileset), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_numTilesets() const", asMETHOD(Tmx::Map, GetNumTilesets), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "bool get_error() const", asMETHOD(Tmx::Map, HasError), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const string& get_errorText() const", asMETHOD(Tmx::Map, GetErrorText), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "int get_errorCode() const", asMETHOD(Tmx::Map, GetErrorCode), asCALL_THISCALL);
  engine->RegisterObjectMethod("Map", "const PropertySet@ get_properties() const", asMETHOD(Tmx::Map, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("Tileset", "int get_firstGid() const", asMETHOD(Tmx::Tileset, GetFirstGid), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const string& get_name() const", asMETHOD(Tmx::Tileset, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_tileWidth() const", asMETHOD(Tmx::Tileset, GetTileWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_tileHeight() const", asMETHOD(Tmx::Tileset, GetTileHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_margin() const", asMETHOD(Tmx::Tileset, GetMargin), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "int get_spacing() const", asMETHOD(Tmx::Tileset, GetSpacing), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const Image@ get_image() const", asMETHOD(Tmx::Tileset, GetImage), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const Tile@ getTile(int) const", asMETHOD(Tmx::Tileset, GetTile), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tileset", "const PropertySet@ get_properties() const", asMETHOD(Tmx::Tileset, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("Tile", "int get_id() const", asMETHOD(Tmx::Tile, GetId), asCALL_THISCALL);
  engine->RegisterObjectMethod("Tile", "const PropertySet@ get_properties() const", asMETHOD(Tmx::Tile, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("Image", "int get_width() const", asMETHOD(Tmx::Image, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Image", "int get_height() const", asMETHOD(Tmx::Image, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Image", "const string& get_source() const", asMETHOD(Tmx::Image, GetSource), asCALL_THISCALL);
  engine->RegisterObjectMethod("Image", "const string& get_transparentColor() const", asMETHOD(Tmx::Image, GetTransparentColor), asCALL_THISCALL);

  engine->RegisterObjectMethod("Layer", "const string& get_name() const", asMETHOD(Tmx::Layer, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "int get_width() const", asMETHOD(Tmx::Layer, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "int get_height() const", asMETHOD(Tmx::Layer, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool get_visible() const", asMETHOD(Tmx::Layer, IsVisible), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "const PropertySet@ get_properties() const", asMETHOD(Tmx::Layer, GetProperties), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "uint getTileId(int, int) const", asMETHOD(Tmx::Layer, GetTileId), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "int getTileTilesetIndex(int, int) const", asMETHOD(Tmx::Layer, GetTileTilesetIndex), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool isTileFlippedHorizontally(int, int) const", asMETHOD(Tmx::Layer, IsTileFlippedHorizontally), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool isTileFlippedVertically(int, int) const", asMETHOD(Tmx::Layer, IsTileFlippedVertically), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "bool isTileFlippedDiagonally(int, int) const", asMETHOD(Tmx::Layer, IsTileFlippedDiagonally), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "const MapTile& getTile(int, int) const", asMETHOD(Tmx::Layer, GetTile), asCALL_THISCALL);
  engine->RegisterObjectMethod("Layer", "int get_zOrder() const", asMETHOD(Tmx::Layer, GetZOrder), asCALL_THISCALL);

  engine->RegisterObjectProperty("MapTile", "int tilesetId", asOFFSET(Tmx::MapTile, tilesetId));
  engine->RegisterObjectProperty("MapTile", "uint id", asOFFSET(Tmx::MapTile, id));
  engine->RegisterObjectProperty("MapTile", "bool flippedHorizontally", asOFFSET(Tmx::MapTile, flippedHorizontally));
  engine->RegisterObjectProperty("MapTile", "bool flippedVertically", asOFFSET(Tmx::MapTile, flippedVertically));
  engine->RegisterObjectProperty("MapTile", "bool flippedDiagonally", asOFFSET(Tmx::MapTile, flippedDiagonally));

  engine->RegisterObjectMethod("Object", "const string& get_name() const", asMETHOD(Tmx::Object, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const string& get_type() const", asMETHOD(Tmx::Object, GetType), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_x() const", asMETHOD(Tmx::Object, GetX), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_y() const", asMETHOD(Tmx::Object, GetY), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_width() const", asMETHOD(Tmx::Object, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_height() const", asMETHOD(Tmx::Object, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "int get_gid() const", asMETHOD(Tmx::Object, GetGid), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const Ellipse@ get_ellipse() const", asMETHOD(Tmx::Object, GetEllipse), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const Polygon@ get_polygon() const", asMETHOD(Tmx::Object, GetPolygon), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const Polyline@ get_polyline() const", asMETHOD(Tmx::Object, GetPolyline), asCALL_THISCALL);
  engine->RegisterObjectMethod("Object", "const PropertySet@ get_properties() const", asMETHOD(Tmx::Object, GetProperties), asCALL_THISCALL);

  engine->RegisterObjectMethod("ObjectGroup", "const string& get_name() const", asMETHOD(Tmx::ObjectGroup, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_width() const", asMETHOD(Tmx::ObjectGroup, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_height() const", asMETHOD(Tmx::ObjectGroup, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "const Object@ getObject(int) const", asMETHOD(Tmx::ObjectGroup, GetObject), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_numObjects() const", asMETHOD(Tmx::ObjectGroup, GetNumObjects), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_visibility() const", asMETHOD(Tmx::ObjectGroup, GetVisibility), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "const PropertySet@ get_properties() const", asMETHOD(Tmx::ObjectGroup, GetProperties), asCALL_THISCALL);
  engine->RegisterObjectMethod("ObjectGroup", "int get_zOrder() const", asMETHOD(Tmx::ObjectGroup, GetZOrder), asCALL_THISCALL);

  engine->RegisterObjectMethod("Ellipse", "int get_centerX() const", asMETHOD(Tmx::Ellipse, GetCenterX), asCALL_THISCALL);
  engine->RegisterObjectMethod("Ellipse", "int get_centerY() const", asMETHOD(Tmx::Ellipse, GetCenterY), asCALL_THISCALL);
  engine->RegisterObjectMethod("Ellipse", "int get_radiusX() const", asMETHOD(Tmx::Ellipse, GetRadiusX), asCALL_THISCALL);
  engine->RegisterObjectMethod("Ellipse", "int get_radiusY() const", asMETHOD(Tmx::Ellipse, GetRadiusY), asCALL_THISCALL);

  engine->RegisterObjectProperty("Point", "float x", asOFFSET(Tmx::Point, x));
  engine->RegisterObjectProperty("Point", "float y", asOFFSET(Tmx::Point, y));

  engine->RegisterObjectMethod("Polygon", "int get_numPoints() const", asMETHOD(Tmx::Polygon, GetNumPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod("Polygon", "const Point& getPoint(int) const", asMETHOD(Tmx::Polygon, GetPoint), asCALL_THISCALL);

  engine->RegisterObjectMethod("Polyline", "int get_numPoints() const", asMETHOD(Tmx::Polyline, GetNumPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod("Polyline", "const Point& getPoint(int) const", asMETHOD(Tmx::Polyline, GetPoint), asCALL_THISCALL);

  engine->RegisterObjectMethod("PropertySet", "int getNumericProperty(const string &in) const", asMETHOD(Tmx::PropertySet, GetNumericProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "float getFloatProperty(const string &in) const", asMETHOD(Tmx::PropertySet, GetFloatProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "const string getLiteralProperty(const string &in) const", asMETHOD(Tmx::PropertySet, GetLiteralProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "int get_size() const", asMETHOD(Tmx::PropertySet, GetSize), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "bool hasProperty(const string &in) const", asMETHOD(Tmx::PropertySet, HasProperty), asCALL_THISCALL);
  engine->RegisterObjectMethod("PropertySet", "int get_empty() const", asMETHOD(Tmx::PropertySet, Empty), asCALL_THISCALL);

  engine->RegisterObjectMethod("ImageLayer", "const string& get_name() const", asMETHOD(Tmx::ImageLayer, GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "int get_width() const", asMETHOD(Tmx::ImageLayer, GetWidth), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "int get_height() const", asMETHOD(Tmx::ImageLayer, GetHeight), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "bool get_visible() const", asMETHOD(Tmx::ImageLayer, IsVisible), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "const Image@ getImage() const", asMETHOD(Tmx::ImageLayer, GetImage), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "const PropertySet@ get_properties() const", asMETHOD(Tmx::ImageLayer, GetProperties), asCALL_THISCALL);
  engine->RegisterObjectMethod("ImageLayer", "int get_zOrder() const", asMETHOD(Tmx::ImageLayer, GetZOrder), asCALL_THISCALL);

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

namespace
{
  template<typename R, typename T, typename C>
  CScriptArray* getArray(T const* t, char const* arrayTypeString, C const& (T::*fp)() const) {
    asIScriptContext* ctx = asGetActiveContext();

    if(ctx != nullptr)
    {
      asIScriptEngine* engine = ctx->GetEngine();
      C const& results = (t->*fp)();
      asIObjectType* arrayType = engine->GetObjectTypeById(engine->GetTypeIdByDecl(arrayTypeString));
      CScriptArray* resultArray = new CScriptArray(results.size(), arrayType);
      int i = 0;
      for(R* result : results)
      {
        resultArray->SetValue(i++, &result);
      }
      return resultArray;
    }
    return nullptr;
  }

  CScriptArray* getMapLayers(Tmx::Map const* map)
  {
    return getArray<Tmx::Layer>(map, "array<tmx::Layer@>", &Tmx::Map::GetLayers);
  }
}

