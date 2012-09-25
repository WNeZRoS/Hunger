#include "LevelMap.h"
#include "Api/Core.h"
#include <cmath>
#include <vector>
#include <stdexcept>

using namespace std;

// LevelMap class

LevelMap::LevelMap(const Tile **map, int width, int height, const Texture::Name tiles,
				   const Point_i& playerSpawn, const Point_i& mobSpawn, Array<Point_i>& foodSpawn) {
	if(!map || !tiles || width <= 0 || height <= 0) throw std::runtime_error("Incorrect arguments");

	_map = const_cast<Tile**>(map);
	_width = width;
	_height = height;
	_tiles = TextureAtlas::Loader(tiles, 8, 4).load();
	_playerSpawn = playerSpawn;
	_mobSpawn = mobSpawn;
	_foodSpawn = foodSpawn;
	_pathFinder = new PathFinder(this);

	_tileSize = 1;
	_xOffset = 0;
	_yOffset = 0;

	if(!_tiles) throw std::runtime_error("Texture not loaded");
}

LevelMap::~LevelMap() {
	for(int i = 0; i < _height; i++) delete [] _map[i];
	delete [] _map;
	delete _pathFinder;
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

void LevelMap::globalToScreen(const Point& global, Point& screen) const {
	screen.x = ( global.x / getOne() + _width / 2.0f) * _tileSize + _xOffset;
	screen.y = ( global.y / getOne() + _height / 2.0f) * _tileSize + _yOffset;
}

void LevelMap::mapToGlobal(const Point_i& map, Point& coord) const {
	coord.x = (1.0f * map.x - _width / 2.0f) * getOne();
#warning                              crash here ^^^
	coord.y = (1.0f * map.y - _height / 2.0f) * getOne();
}

void LevelMap::globalToMap(const Point& coord, Point_i& map) const {
	Point mapf(coord.x / getOne() + _width / 2.0f, coord.y / getOne() + _height / 2.0f);
	if(mapf.x < 0) mapf.x = 0; else if(mapf.x >= _width) mapf.x = _width - 1;
	if(mapf.y < 0) mapf.y = 0; else if(mapf.y >= _height) mapf.y = _height - 1;
	map = mapf.toPoint_i();
}

const Tile& LevelMap::getTileByCoords(const Point_i& coord) const {
	return _map[coord.y][coord.x];
}

Point LevelMap::getPlayerSpawnPosition() const {
	Point pos;
	this->mapToGlobal(_playerSpawn, pos);
	return pos;
}

Point LevelMap::getMobSpawnPosition() const {
	Point pos;
	this->mapToGlobal(_mobSpawn, pos);
	return pos;
}

Array<Point> LevelMap::getFoodSpawns() const {
	Array<Point> foodSpawn(_foodSpawn.size());
	for(unsigned int i = 0; i < foodSpawn.size(); i++) {
		this->mapToGlobal(_foodSpawn[i], foodSpawn[i]);
	}
	return foodSpawn;
}

void LevelMap::getRoads(Array<Point>& roads) const {
	vector<Point> vRoads;
	for(Point_i i(1,1); i.y < _height - 1; i.y++)
		for(i.x = 1; i.x < _width - 1; i.x++)
			if(_playerSpawn != i && _map[i.y][i.x].isRoad() && !_map[i.y][i.x].isClear() && !_foodSpawn.contains(i)) {
				Point point;
				this->mapToGlobal(i, point);
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

void LevelMap::openGates() {
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {
			if(_map[y][x].isGate()) _map[y][x].openGate();
		}
	}
}

void LevelMap::closeGates() {
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {
			if(_map[y][x].isGate()) _map[y][x].closeGate();
		}
	}
}

bool LevelMap::isRoad(const Point &globalPos) const {
	Point_i tilePos;
	this->globalToMap(globalPos, tilePos);
	return getTileByCoords(tilePos).isRoad();
}

void LevelMap::turnToBounds(Point &globalPos) const {
	if(globalPos.x <= - _width * getOne() / 2.0f) globalPos.x += _width * getOne();
	else if(globalPos.x >= _width * getOne() / 2.0f) globalPos.x -= _width * getOne();
	if(globalPos.y <= - _height * getOne() / 2.0f) globalPos.y += _height * getOne();
	else if(globalPos.y >= _height * getOne() / 2.0f) globalPos.y -= _height * getOne();
}

void LevelMap::turnToBounds(Point_i &mapPos) const {
	while(mapPos.x < 0) mapPos.x += _width;
	while(mapPos.y < 0) mapPos.y += _height;
	while(mapPos.x >= _width) mapPos.x -= _width;
	while(mapPos.y >= _height) mapPos.y -= _height;
}

PathFinder * LevelMap::getPathFinder() const {
	return this->_pathFinder;
}
