#include "LevelMap.h"
#include "Api/FileManager.h"
#include <cmath>
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

bool LevelMap::Tile::haveRoadAt(const Point& pos, int tileSize) const {
	if(pos.x < 0 || pos.y < 0 || pos.y > tileSize || pos.x > tileSize)
		return false;

	float halfSize = 0.5f * tileSize;

	switch(_type) {
	case TOP_LEFT:
	case CL_TOP_LEFT:
		return isCenterBottom(pos, halfSize) || isRightCenter(pos, halfSize);
	case T_DOWN:
	case CL_T_DOWN:
		return isCenterBottom(pos, halfSize) || equal(pos.y, halfSize);
	case TOP_RIGHT:
	case CL_TOP_RIGHT:
		return isCenterBottom(pos, halfSize) || isLeftCenter(pos, halfSize);
	case ROAD_LEFT:
		return equal(pos.y, halfSize);
	case ROAD_UP:
		return equal(pos.x, halfSize);
	case T_LEFT:
	case CL_T_LEFT:
		return isRightCenter(pos, halfSize) || equal(pos.x, halfSize);
	case CROSSROAD:
	case CL_CROSSROAD:
	case CL_CROSSROAD_LEFT:
	case CL_CROSSROAD_TOP:
	case CL_CROSSROAD_BOTTOM:
	case CL_CROSSROAD_RIGHT:
		return pos.x == halfSize || equal(pos.y, halfSize);
	case T_RIGHT:
	case CL_T_RIGHT:
		return isLeftCenter(pos, halfSize) || equal(pos.x, halfSize);
	case BOTTOM_LEFT:
	case CL_BOTTOM_LEFT:
		return isCenterUp(pos, halfSize) || isRightCenter(pos, halfSize);
	case T_UP:
	case CL_T_UP:
		return isCenterUp(pos, halfSize) || equal(pos.y, halfSize);
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

bool LevelMap::Tile::canSetToRoad(Point& pos, int tileSize) const {
	if(this->haveRoadAt(pos, tileSize)) return true;
	float halfSize = 0.5f * tileSize;
	float dx = ABS(pos.x - halfSize), dy = ABS(pos.y - halfSize);
	//float maxd = tileSize / 16.0f;


	bool hx = this->haveRoadAt(Point(halfSize, pos.y), tileSize);
	bool hy = this->haveRoadAt(Point(pos.x, halfSize), tileSize);

	if(hx)
		if(hy)
			if(dx < dy) pos.x = halfSize;
			else pos.y = halfSize;
		else pos.x = halfSize;
	else if(hy) pos.y = halfSize;

	return hx || hy;
}

bool LevelMap::Tile::isCenterUp(const Point& pos, float halfSize) const {
	return equal(pos.x, halfSize) && pos.y <= halfSize;
}

bool LevelMap::Tile::isCenterBottom(const Point& pos, float halfSize) const {
	return equal(pos.x, halfSize) && pos.y >= halfSize;
}

bool LevelMap::Tile::isLeftCenter(const Point& pos, float halfSize) const {
	return pos.x <= halfSize && equal(pos.y, halfSize);
}

bool LevelMap::Tile::isRightCenter(const Point& pos, float halfSize) const {
	return pos.x >= halfSize && equal(pos.y, halfSize);
}

// LevelMap class

LevelMap::LevelMap(const Tile **map, int width, int height, const Texture::Name tiles,
				   const Point_i& playerSpawn, const Point_i& mobSpawn, Array<Point_i>& foodSpawn) {
	if(!map || !tiles || width <= 0 || height <= 0) throw;

	_map = map;
	_width = width;
	_height = height;
	_tiles = TextureAtlas::Loader(tiles, 8, 4).load();
	_playerSpawn = playerSpawn;
	_mobSpawn = mobSpawn;
	_foodSpawn = foodSpawn;

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
	ifstream *file = FileManager::instance().read(filename);
	if(!file || !file->is_open()) {
		Log::logger << Log::error << "Can't load file: " << filename;
		if(file) delete file;
		return NULL;
	}
	int width, height;
	*file >> width >> height;

	if(width <= 0 || height <= 0) {
		Log::logger << Log::error << "Width/Height <= 0";
		delete file;
		return NULL;
	}

	Tile **map = new Tile*[height];

	for(int y = 0; y < height; y++) {
		map[y] = new Tile[width];
		for(int x = 0; x < width; x++) *file >> map[y][x];
	}

	Point_i playerSpawn, mobSpawn;
	*file >> playerSpawn >> mobSpawn;

	Array<Point_i> foodSpawn;
	*file >> foodSpawn.size;
	foodSpawn.rebuild(foodSpawn.size);
	for(unsigned int i = 0; i < foodSpawn.size; i++)
		*file >> foodSpawn[i];

	delete file;
	return new LevelMap(const_cast<const Tile**>(map), width, height, tiles, playerSpawn, mobSpawn, foodSpawn);
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

void LevelMap::setCenter(float x, float y) {
	if(_xOffset <= 0) {
		_xOffset = (_screenWidth - _tileSize * _width ) / 2 - x;
		if(_xOffset > 0) _xOffset = 0;
		else if(_xOffset < _screenWidth - _tileSize * _width)
			_xOffset = _screenWidth - _tileSize * _width;
	}
	if(_yOffset <= 0) {
		_yOffset = (_screenHeight - _tileSize * _height) / 2 - y;
		if(_yOffset > 0) _yOffset = 0;
		else if(_yOffset < _screenHeight - _tileSize * _height)
			_yOffset = _screenHeight - _tileSize * _height;
	}
}

float LevelMap::getOne() const {
	return 10.0f;
}

void LevelMap::globalCoordinatesToScreen(const Point& global, Point& screen) const {
	//(1.0f * map.x - _width / 2.0f) =
	// _xOffset + (x * _tileSize) = _xOffset + ( ( px + _width / 2.0f ) * _tileSize )
	screen.x = ( global.x / getOne() + _width / 2.0f) * _tileSize + _xOffset;
	screen.y = ( global.y / getOne() + _height / 2.0f) * _tileSize + _yOffset;
}

void LevelMap::globalCoordinatesToMap(const Point& coord, Point_i& map) const {
	Point mapf(coord.x / getOne() + _width / 2.0f, coord.y / getOne() + _height / 2.0f);
	if(mapf.x < 0) mapf.x = 0; else if(mapf.x >= _width) mapf.x = _width - 1;
	if(mapf.y < 0) mapf.y = 0; else if(mapf.y >= _height) mapf.y = _height - 1;
	map = mapf.toPoint_i();
}

void LevelMap::mapCoordinatesToGlobal(const Point_i& map, Point& coord) const {
	coord.x = (1.0f * map.x - _width / 2.0f) * getOne();
	coord.y = (1.0f * map.y - _height / 2.0f) * getOne();
}

const LevelMap::Tile& LevelMap::getTileByCoords(const Point_i& coord) const {
	return _map[coord.y][coord.x];
}

Point LevelMap::getPlayerSpawnPosition() const {
	Point pos;
	mapCoordinatesToGlobal(_playerSpawn, pos);
	return pos;
}

Point LevelMap::getMobSpawnPosition() const {
	Point pos;
	mapCoordinatesToGlobal(_mobSpawn, pos);
	return pos;
}

Array<Point> LevelMap::getFoodSpawns() const {
	Array<Point> foodSpawn(_foodSpawn.size);
	for(unsigned int i = 0; i < foodSpawn.size; i++) {
		mapCoordinatesToGlobal(_foodSpawn[i], foodSpawn[i]);
	}
	return foodSpawn;
}

void LevelMap::getRoads(Array<Point>& roads) const {
	vector<Point> vRoads;
	for(Point_i i(1,1); i.y < _height - 1; i.y++)
		for(i.x = 1; i.x < _width - 1; i.x++)
			if(_playerSpawn != i && _map[i.y][i.x].isRoad() && !_foodSpawn.contains(i)) {
				Point point;
				mapCoordinatesToGlobal(i, point);
				//Log::logger << Log::debug << i << " -> " << point;
				vRoads.push_back(point);
			}

	roads.copy(&vRoads.at(0), vRoads.size());
}

void LevelMap::draw() const {
	if(!_map || !_tiles) return;

	for(int y = -_yOffset / _tileSize - 1; y * _tileSize <= _screenHeight; y++) {
		for(int x = -_xOffset / _tileSize - 1; x * _tileSize <= _screenWidth; x++) {
			_tiles->drawTile(_map[y >= _height ? _height - 1 : (y < 0 ? 0 : y)]
												 [x >= _width ? _width - 1 : (x < 0 ? 0 : x)].type(),
							 _xOffset + x * _tileSize, _yOffset + y * _tileSize, 0, _tileSize, _tileSize);
		}
	}
}

void LevelMap::getPositions(const Point& pos, Point_i& tilePos, Point& roadPos) const {
	this->globalCoordinatesToMap(pos, tilePos);
	this->mapCoordinatesToGlobal(tilePos, roadPos);
	roadPos = pos - roadPos;
}

bool LevelMap::moveInDirection(Point& from, const Point &dir, float speed) const {
	Point_i fromTilePos;
	Point roadPos;
	this->getPositions(from, fromTilePos, roadPos);

	Log::logger << Log::debug << "from tile " << getTileByCoords(fromTilePos).type() << " "
				<< fromTilePos << " road pos " << roadPos;

	//if(!getTileByCoords(fromTilePos).haveRoadAt(roadPos, _tileSize)) return false;

	Point newPos = from + dir * speed;
	if(newPos.x <= - _width * getOne() / 2.0f) newPos.x += _width * getOne();
	else if(newPos.x >= _width * getOne() / 2.0f) newPos.x -= _width * getOne();
	if(newPos.y <= - _height * getOne() / 2.0f) newPos.y += _height * getOne();
	else if(newPos.y >= _height * getOne() / 2.0f) newPos.y -= _height * getOne();

	Point_i newTilePos;
	Point newRoadPos;
	this->getPositions(newPos, newTilePos, newRoadPos);

	Log::logger << Log::debug << "new tile " << getTileByCoords(newTilePos).type() << " "
				<< newTilePos << " road pos " << newRoadPos;

	if(getTileByCoords(newTilePos).haveRoadAt(newRoadPos, getOne())) {
		from = newPos;
		return true;
	}

	roadPos = newRoadPos;
	if(getTileByCoords(newTilePos).canSetToRoad(roadPos, getOne())) {
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

float LevelMap::distance(Point tile1, const Point &tile2) const {
	return std::max(ABS(tile1.x - tile2.x), ABS(tile1.y - tile2.y));
}
