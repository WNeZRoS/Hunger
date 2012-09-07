#include "Api/compatibility.h"
#include "Api/Logger.h"
#include "LevelMap.h"
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;

// Tile class
LevelMap::Tile::Tile() {
	_type = WALL;
}

LevelMap::Tile::Tile(TileType type) {
	_type = type;
}

LevelMap::Tile::~Tile() {

}

const LevelMap::Tile::TileType& LevelMap::Tile::type() const {
	return _type;
}

istream& operator >> (istream& in, LevelMap::Tile& t) {
	in >> (int&)t._type;
	return in;
}

bool LevelMap::Tile::haveRoadAt(const Point &pos, int tileSize) const {
	if(pos.x < 0 || pos.y < 0 || pos.y > tileSize || pos.x > tileSize)
		return false;

	int halfSize = tileSize / 2;

	switch(_type) {
	case TOP_LEFT:
	case CL_TOP_LEFT:
		return isCenterBottom(pos, halfSize) || isRightCenter(pos, halfSize);
	case T_DOWN:
	case CL_T_DOWN:
		return isCenterBottom(pos, halfSize) || pos.y == halfSize;
	case TOP_RIGHT:
	case CL_TOP_RIGHT:
		return isCenterBottom(pos, halfSize) || isLeftCenter(pos, halfSize);
	case ROAD_LEFT:
		return pos.y == halfSize;
	case ROAD_UP:
		return pos.x == halfSize;
	case T_LEFT:
	case CL_T_LEFT:
		return isRightCenter(pos, halfSize) || pos.x == halfSize;
	case CROSSROAD:
	case CL_CROSSROAD:
	case CL_CROSSROAD_LEFT:
	case CL_CROSSROAD_TOP:
	case CL_CROSSROAD_BOTTOM:
	case CL_CROSSROAD_RIGHT:
		return pos.x == halfSize || pos.y == halfSize;
	case T_RIGHT:
	case CL_T_RIGHT:
		return isLeftCenter(pos, halfSize) || pos.x == halfSize;
	case BOTTOM_LEFT:
	case CL_BOTTOM_LEFT:
		return isCenterUp(pos, halfSize) || isRightCenter(pos, halfSize);
	case T_UP:
	case CL_T_UP:
		return isCenterUp(pos, halfSize) || pos.y == halfSize;
	case BOTTOM_RIGHT:
	case CL_BOTTOM_RIGHT:
		return isCenterUp(pos, halfSize) || isLeftCenter(pos, halfSize);
	default:
		return false;
	}
	return false;
}

bool LevelMap::Tile::isRoad() const {
	return _type == ROAD_LEFT || _type == ROAD_UP || _type == TOP_LEFT || _type == TOP_RIGHT
			|| _type == T_DOWN || _type == BOTTOM_LEFT || _type == BOTTOM_RIGHT || _type == T_UP
			|| _type == T_LEFT || _type == T_RIGHT || _type == CROSSROAD;
}

bool LevelMap::Tile::canSetToRoad(Point &pos, int tileSize) const {
	int halfSize = tileSize / 2.0f;
	int dx = ABS(pos.x - halfSize), dy = ABS(pos.y - halfSize);

	bool result = false;
	if(dx > 0 && dx < tileSize / 16.0f) {
		pos.x = halfSize;
		result = true;
	}
	if(dy > 0 && dy < tileSize / 16.0f) {
		pos.y = halfSize;
		result = true;
	}
	return result;
}

bool LevelMap::Tile::isCenterUp(const Point& pos, int halfSize) const {
	return pos.x == halfSize && pos.y <= halfSize;
}

bool LevelMap::Tile::isCenterBottom(const Point& pos, int halfSize) const {
	return pos.x == halfSize && pos.y >= halfSize;
}

bool LevelMap::Tile::isLeftCenter(const Point& pos, int halfSize) const {
	return pos.x <= halfSize && pos.y == halfSize;
}

bool LevelMap::Tile::isRightCenter(const Point& pos, int halfSize) const {
	return pos.x >= halfSize && pos.y == halfSize;
}

// LevelMap class

LevelMap::LevelMap(const Tile **map, int width, int height, const Texture::Name tiles,
				   const Point& playerSpawn, const Point& mobSpawn) {
	if(!map || !tiles || width <= 0 || height <= 0) throw;

	_map = map;
	_width = width;
	_height = height;
	_tiles = TextureAtlas::Loader(tiles, 8, 4).load();
	_playerSpawn = playerSpawn;
	_mobSpawn = mobSpawn;

	_tileSize = 1;
	_xOffset = 0;
	_yOffset = 0;

	if(!_tiles) throw;
}

LevelMap::~LevelMap() {
	for(int i = 0; i < _height; i++) delete _map[i];
	delete _map;
	_tiles->unload();
}

LevelMap * LevelMap::load(const char *filename, const Texture::Name tiles) {
	ifstream file(filename, ios::in);
	if(!file.is_open()) {
		Log::logger << Log::error << "Can't load file: " << filename;
		return NULL;
	}
	int width, height;
	file >> width >> height;

	if(width <= 0 || height <= 0) {
		Log::logger << Log::error << "Width/Height <= 0";
		return NULL;
	}

	Tile **map = new Tile*[height];

	for(int y = 0; y < height; y++) {
		map[y] = new Tile[width];
		for(int x = 0; x < width; x++) file >> map[y][x];
	}

	Point playerSpawn, mobSpawn;
	file >> playerSpawn.x >> playerSpawn.y;
	file >> mobSpawn.x >> mobSpawn.y;

	return new LevelMap(const_cast<const Tile**>(map), width, height, tiles, playerSpawn, mobSpawn);
}

void LevelMap::fillScreen(int width, int height, int minTileSize) {
	int widthSize = width / _width;
	int heightSize = height / _height;
	int tileSize = widthSize < heightSize ? widthSize : heightSize;
	setTileSize(tileSize < minTileSize ? minTileSize : tileSize);
	_xOffset = (width - _tileSize * _width ) / 2;
	_yOffset = (height - _tileSize * _height) / 2;
	_screenWidth = width;
	_screenHeight = height;
}

void LevelMap::setTileSize(int size) {
	_tileSize = size;
}

void LevelMap::setCenter(int x, int y) {
	if(_xOffset < 0) {
		_xOffset = (_screenWidth - _tileSize * _width ) / 2 + x;
		if(_xOffset > 0) _xOffset = 0;
		else if(_xOffset < _screenWidth - _tileSize * _width)
			_xOffset = _screenWidth - _tileSize * _width;
	}
	if(_yOffset < 0) {
		_yOffset = (_screenHeight - _tileSize * _height) / 2 + y;
		if(_yOffset > 0) _yOffset = 0;
		else if(_yOffset < _screenHeight - _tileSize * _height)
			_yOffset = _screenHeight - _tileSize * _height;
	}
}

void LevelMap::globalCoordinatesToMap(const Point& coord, Point& map) const {
	map.x = (_tileSize * _width / 2.0f) + coord.x;
	map.y = (_tileSize * _height / 2.0f) + coord.y;
	map.x /= _tileSize;
	map.y /= _tileSize;
	if(map.x < 0) map.x = 0; else if(map.x >= _width) map.x = _width - 1;
	if(map.y < 0) map.y = 0; else if(map.y >= _height) map.y = _height - 1;
}

void LevelMap::globalCoordinatesToScreen(const Point& global, Point& screen) const {
	screen.x = _xOffset + (_tileSize * _width / 2.0f) + global.x;
	screen.y = _yOffset + (_tileSize * _height / 2.0f) + global.y;
}

void LevelMap::mapCoordinatesToGlobal(const Point& map, Point& coord) const {
	coord.x = _tileSize * (map.x - _width / 2.0f);
	coord.y = _tileSize * (map.y - _height / 2.0f);
}

const LevelMap::Tile& LevelMap::getTileByCoords(const Point& coord) const {
	return _map[coord.y][coord.x];
}

Point LevelMap::getPlayerSpawnPosition() const {
	Point pos(_playerSpawn.x, _playerSpawn.y);
	mapCoordinatesToGlobal(pos, pos);
	return pos;
}

Point LevelMap::getMobSpawnPosition() const {
	Point pos(_mobSpawn.x * _tileSize, _mobSpawn.y * _tileSize);
	mapCoordinatesToGlobal(pos, pos);
	return pos;
}

void LevelMap::getRoads(Point*& roads, int& size) const {
	vector<Point> vRoads;
	for(int y = 1; y < _height - 1; y++)
		for(int x = 1; x < _width - 1; x++)
			if((_playerSpawn.x != x || _playerSpawn.y != y) && _map[y][x].isRoad()) {
				Point point(x, y);
				mapCoordinatesToGlobal(point, point);
				vRoads.push_back(point);
			}

	roads = vRoads.data();
	size = vRoads.size();
}

void LevelMap::draw() const {
	if(!_map || !_tiles) return;

	for(int y = -_yOffset / _tileSize - 1; y + _tileSize > 0 && y <= _height; y++) {
		for(int x = -_xOffset / _tileSize - 1; x + _tileSize > 0 && x <= _width; x++) {

			_tiles->drawTile(_map[y >= _height ? _height - 1 : (y < 0 ? 0 : y)]
												 [x >= _width ? _width - 1 : (x < 0 ? 0 : x)].type(),
							 _xOffset + x * _tileSize, _yOffset + y * _tileSize, 0, _tileSize, _tileSize);
		}
	}
}

void LevelMap::getPositions(const Point& pos, Point& tilePos, Point& roadPos) const {
	this->globalCoordinatesToMap(pos, tilePos);
	this->mapCoordinatesToGlobal(tilePos, roadPos);
	roadPos = pos - roadPos;
}

bool LevelMap::moveInDirection(Point& from, const Point &dir, float speed) const {
	Point fromTilePos, roadPos;
	this->getPositions(from, fromTilePos, roadPos);

	Log::logger << Log::debug << "from tile " << getTileByCoords(fromTilePos).type() << " "
				<< fromTilePos << " road pos " << roadPos << " " << _tileSize;

	if(!getTileByCoords(fromTilePos).haveRoadAt(roadPos, _tileSize)) return false;


	Point newPos = from + dir * speed;
	if(newPos.x <= - _width * _tileSize / 2.0f) newPos.x += _width * _tileSize;
	else if(newPos.x >= _width * _tileSize / 2.0f) newPos.x -= _width * _tileSize;
	if(newPos.y <= - _height * _tileSize / 2.0f) newPos.y += _height * _tileSize;
	else if(newPos.y >= _height * _tileSize / 2.0f) newPos.y -= _height * _tileSize;

	Point newTilePos, newRoadPos;
	this->getPositions(newPos, newTilePos, newRoadPos);

	Log::logger << Log::debug << "new tile " << getTileByCoords(newTilePos).type() << " "
				<< newTilePos << " road pos " << newRoadPos;

	if(getTileByCoords(newTilePos).haveRoadAt(newRoadPos, _tileSize)) {
		from = newPos;
		return true;
	}

	roadPos = newRoadPos;
	if(getTileByCoords(newTilePos).canSetToRoad(roadPos, _tileSize)) {
		Log::logger << Log::debug << "delta road " << (roadPos.x - newRoadPos.x) << " " << (roadPos.y - newRoadPos.y);
		Point newPos2 = newPos + roadPos - newRoadPos;
		if(newPos != newPos2 && newPos2 != from) {
			from = newPos2;
			return true;
		}
		return false;
	}

	return false;
}

int LevelMap::distance(Point tile1, const Point &tile2) const {
	return std::max(ABS(tile1.x - tile2.x), ABS(tile1.y - tile2.y));
}
