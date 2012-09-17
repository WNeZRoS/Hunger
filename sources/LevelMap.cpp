#include "LevelMap.h"
#include "Api/FileManager.h"
#include <cmath>
#include <vector>
#include <stdexcept>

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
	return _type != WALL && _type != GATE_CROSSROAD_LEFT && _type != GATE_CROSSROAD_TOP
			&& _type != GATE_CROSSROAD_BOTTOM && _type != GATE_CROSSROAD_RIGHT && _type != FREE_1
			&& _type != FREE_2 && _type != FREE_3;
}

bool LevelMap::Tile::isClear() const {
	return _type == CL_TOP_LEFT || _type == CL_T_DOWN || _type == CL_TOP_RIGHT || _type == CL_T_LEFT
			|| _type == CL_CROSSROAD || _type == CL_T_RIGHT || _type == CL_BOTTOM_LEFT
			|| _type == CL_T_UP || _type == CL_BOTTOM_RIGHT || _type == CL_CROSSROAD_LEFT
			|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT;
}

bool LevelMap::Tile::isRoadUp() const {
	return _type == ROAD_UP || _type == T_UP || _type == BOTTOM_LEFT || _type == BOTTOM_RIGHT
			|| _type == T_LEFT || _type == T_RIGHT || _type == CROSSROAD || _type == CL_T_UP
			|| _type == CL_T_LEFT || _type == CL_CROSSROAD || _type == CL_T_RIGHT
			|| _type == CL_BOTTOM_LEFT || _type == CL_BOTTOM_RIGHT || _type == CL_CROSSROAD_LEFT
			|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT;
}

bool LevelMap::Tile::isRoadDown() const {
	return _type == ROAD_UP || _type == TOP_LEFT || _type == TOP_RIGHT || _type == T_DOWN
			|| _type == T_LEFT || _type == T_RIGHT || _type == CROSSROAD || _type == CL_TOP_LEFT
			|| _type == CL_TOP_RIGHT || _type == CL_T_DOWN || _type == CL_T_LEFT
			|| _type == CL_T_RIGHT || _type == CL_CROSSROAD || _type == CL_CROSSROAD_LEFT
			|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT;
}

bool LevelMap::Tile::isRoadLeft() const {
	return _type == ROAD_LEFT || _type == TOP_RIGHT || _type == T_DOWN || _type == BOTTOM_RIGHT
			|| _type == T_UP || _type == T_RIGHT || _type == CROSSROAD || _type == CL_TOP_RIGHT
			|| _type == CL_T_DOWN || _type == CL_BOTTOM_RIGHT || _type == CL_T_UP
			|| _type == CL_T_RIGHT || _type == CL_CROSSROAD || _type == CL_CROSSROAD_LEFT
			|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT;
}

bool LevelMap::Tile::isRoadRight() const {
	return _type == ROAD_LEFT  || _type == TOP_LEFT || _type == T_DOWN || _type == BOTTOM_LEFT
			|| _type == T_UP || _type == T_LEFT ||  _type == CROSSROAD || _type == CL_TOP_LEFT
			|| _type == CL_T_DOWN || _type == CL_BOTTOM_LEFT || _type == CL_T_UP
			|| _type == CL_T_LEFT || _type == CL_CROSSROAD || _type == CL_CROSSROAD_LEFT
			|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT;
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
	if(!map || !tiles || width <= 0 || height <= 0) throw std::runtime_error("Incorrect arguments");

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

	if(!_tiles) throw std::runtime_error("Texture not loaded");
}

LevelMap::~LevelMap() {
	for(int i = 0; i < _height; i++) delete [] _map[i];
	delete [] _map;
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

	unsigned int foodSpawnSize;
	Array<Point_i> foodSpawn;
	*file >> foodSpawnSize;
	foodSpawn.rebuild(foodSpawnSize);
	for(unsigned int i = 0; i < foodSpawn.size(); i++)
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
	Array<Point> foodSpawn(_foodSpawn.size());
	for(unsigned int i = 0; i < foodSpawn.size(); i++) {
		mapCoordinatesToGlobal(_foodSpawn[i], foodSpawn[i]);
	}
	return foodSpawn;
}

void LevelMap::getRoads(Array<Point>& roads) const {
	vector<Point> vRoads;
	for(Point_i i(1,1); i.y < _height - 1; i.y++)
		for(i.x = 1; i.x < _width - 1; i.x++)
			if(_playerSpawn != i && _map[i.y][i.x].isRoad() && !_map[i.y][i.x].isClear() && !_foodSpawn.contains(i)) {
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

	//Log::logger << Log::debug << "from tile " << getTileByCoords(fromTilePos).type() << " "
	//			<< fromTilePos << " road pos " << roadPos;

	//if(!getTileByCoords(fromTilePos).haveRoadAt(roadPos, _tileSize)) return false;

	Point newPos = from + dir * speed;
	this->turnToBounds(newPos);

	Point_i newTilePos;
	Point newRoadPos;
	this->getPositions(newPos, newTilePos, newRoadPos);

	//Log::logger << Log::debug << "new tile " << getTileByCoords(newTilePos).type() << " "
	//			<< newTilePos << " road pos " << newRoadPos;

	if(getTileByCoords(newTilePos).haveRoadAt(newRoadPos, getOne())) {
		from = newPos;
		return true;
	}

	roadPos = newRoadPos;
	if(getTileByCoords(newTilePos).canSetToRoad(roadPos, getOne())) {
		//Log::logger << Log::debug << "delta road " << (roadPos.x - newRoadPos.x) << " " << (roadPos.y - newRoadPos.y);
		Point newPos2 = newPos + roadPos - newRoadPos;
		if(newPos != newPos2 && newPos2 != from) {
			from = newPos2;
			return true;
		}
		return false;
	}

	return false;
}

unsigned int LevelMap::findMapPath(const Point_i& pfrom, const Point_i& to, const Point_i& noCross,
								   unsigned int **&wasHere, unsigned int step) const {
	Point_i from = pfrom;
	if(from.x < 0 || from.x >= _width) from.x = (from.x + _width) % _width;
	if(from.y < 0 || from.y >= _height) from.y = (from.y + _height) % _height;
	if(wasHere[from.y][from.x] <= step) return -1;
	if(!this->getTileByCoords(from).isRoad()) return -1;
	if(from == noCross) return -1;
	if(from == to) {
		wasHere[from.y][from.x] = step;
		return step;
	}

	static const Point_i vertical(0, 1), horizont(1, 0);

	unsigned int prevState = wasHere[from.y][from.x];
	wasHere[from.y][from.x] = step;
	unsigned int min = -1;
	if(this->getTileByCoords(from).isRoadUp()) {
		unsigned int size = findMapPath(from - vertical, to, noCross, wasHere, step + 1);
		if(size < min) min = size;
	}

	if(this->getTileByCoords(from).isRoadDown()) {
		unsigned int size = findMapPath(from + vertical, to, noCross, wasHere, step + 1);
		if(size < min) min = size;
	}

	if(this->getTileByCoords(from).isRoadLeft()) {
		unsigned int size = findMapPath(from - horizont, to, noCross, wasHere, step + 1);
		if(size < min) min = size;
	}

	if(this->getTileByCoords(from).isRoadRight()) {
		unsigned int size = findMapPath(from + horizont, to, noCross, wasHere, step + 1);
		if(size < min) min = size;
	}

	if(min == static_cast<unsigned int>(-1)) {
		wasHere[from.y][from.x] = prevState;
	}

	return min;
}

void LevelMap::findPath(const Point &from, const Point &to, Array<PathSegment> &path) const {
	this->findPath(from, to, path, Point(NAN, NAN));
}

void LevelMap::backPath(const Point_i& pto, Array<PathSegment> &path, unsigned int **wasHere, unsigned int step) const {
	Point_i to = pto;
	if(to.x < 0 || to.x >= _width) to.x = (to.x + _width) % _width;
	if(to.y < 0 || to.y >= _height) to.y = (to.y + _height) % _height;
	if(wasHere[to.y][to.x] != step) return;

	PathSegment ps;
	this->mapCoordinatesToGlobal(to, ps.target);
	path[step - 1] = ps;

	if(step > 1) {
		static const Point_i vertical(0, 1), horizont(1, 0);
		backPath(to - vertical, path, wasHere, step - 1);
		backPath(to + vertical, path, wasHere, step - 1);
		backPath(to - horizont, path, wasHere, step - 1);
		backPath(to + horizont, path, wasHere, step - 1);
	}
}

void LevelMap::findPath(const Point& from, const Point& to, Array<PathSegment> &path, const Point& doNoCross) const {
	Point_i tileFrom, tileTo, tileNoCross(-1, -1);
	this->globalCoordinatesToMap(from, tileFrom);
	this->globalCoordinatesToMap(to, tileTo);
	if(!doNoCross.nan()) this->globalCoordinatesToMap(doNoCross, tileNoCross);

	unsigned int **wasHere = new unsigned int*[_height];
	for(int i = 0; i < _height; i++) {
		wasHere[i] = new unsigned int[_width];
		for(int j = 0; j < _width; j++) wasHere[i][j] = -1;
	}

	unsigned int size = findMapPath(tileFrom, tileTo, tileNoCross, wasHere, 0);
	if(size > 0 && size != static_cast<unsigned int>(-1)) {
		path.rebuild(size);
		backPath(tileTo, path, wasHere, size);
	}

	for(int i = 0; i < _height; i++) delete [] wasHere;
	delete wasHere;
}

bool LevelMap::isOneTile(const Point& globalPos1, const Point& globalPos2) const {
	Point_i tilePos1, tilePos2;
	this->globalCoordinatesToMap(globalPos1, tilePos1);
	this->globalCoordinatesToMap(globalPos2, tilePos2);
	return tilePos1 == tilePos2;
}

bool LevelMap::isRoad(const Point &globalPos) const {
	Point_i tilePos;
	this->globalCoordinatesToMap(globalPos, tilePos);
	return getTileByCoords(tilePos).isRoad();
}

void LevelMap::turnToBounds(Point &globalPos) const {
	if(globalPos.x <= - _width * getOne() / 2.0f) globalPos.x += _width * getOne();
	else if(globalPos.x >= _width * getOne() / 2.0f) globalPos.x -= _width * getOne();
	if(globalPos.y <= - _height * getOne() / 2.0f) globalPos.y += _height * getOne();
	else if(globalPos.y >= _height * getOne() / 2.0f) globalPos.y -= _height * getOne();
}
