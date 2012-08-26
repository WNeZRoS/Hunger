#include "Api/compatibility.h"
#include "LevelMap.h"
#include <iostream>
#include <stdio.h> // TODO make on std

using namespace std;

LevelMap::LevelMap(const TileType **map, int width, int height, const Texture *tiles,
				   const Point& playerSpawn, const Point& mobSpawn) {
	if(!map || !tiles || width <= 0 || height <= 0) throw;

	_map = map;
	_width = width;
	_height = height;
	_tiles = tiles->toAtlas(8, 4);
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
	if(_tiles) delete _tiles;
}

LevelMap * LevelMap::load(const char * filename, const Texture *tiles) {
	FILE * file = fopen(filename, "r");
	if(!file) {
		printf("Can't load file: %s\n", filename);
		return NULL;
	}
	int width, height;
	fscanf(file, "%d %d", &width, &height);

	if(width <= 0 || height <= 0) {
		printf("Width/Height <= 0\n");
		return NULL;
	}

	TileType ** map = new TileType*[height];

	for(int y = 0; y < height; y++) {
		map[y] = new TileType[width];
		for(int x = 0; x < width; x++) {
			fscanf(file, "%d", (int*)&map[y][x]);
		}
	}

	Point playerSpawn, mobSpawn;
	fscanf(file, "%d %d", &playerSpawn.x, &playerSpawn.y);
	fscanf(file, "%d %d", &mobSpawn.x, &mobSpawn.y);

	return new LevelMap(const_cast<const TileType**>(map), width, height, tiles, playerSpawn, mobSpawn);
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
	map.x = (_tileSize * _width / 2) + coord.x;
	map.y = (_tileSize * _height / 2) + coord.y;
}

void LevelMap::globalCoordinatesToScreen(const Point& global, Point& screen) const {
	screen.x = _xOffset + (_tileSize * _width / 2) + global.x;
	screen.y = _yOffset + (_tileSize * _height / 2) + global.y;
}

void LevelMap::mapCoordinatesToGlobal(const Point& map, Point& coord) const {
	coord.x = map.x - (_tileSize * _width / 2);
	coord.y = map.y - (_tileSize * _height / 2);
}

const LevelMap::TileType& LevelMap::getTileByCoords(int x, int y) const {
	getTileByCoords(x, y, x, y);
	return _map[y][x];
}

void LevelMap::getTileByCoords(int x, int y, int& tx, int& ty) const {
	Point pos = {x, y};
	globalCoordinatesToMap(pos, pos);
	pos.x /= _tileSize;
	pos.y /= _tileSize;
	if(pos.x < 0) pos.x = 0;
	if(pos.y < 0) pos.y = 0;
	if(pos.x >= _width) pos.x = _width - 1;
	if(pos.y >= _height) pos.y = _height - 1;
	tx = pos.x;
	ty = pos.y;
}

void LevelMap::getPlayerSpawnPosition(int& x, int& y) const {
	Point pos = {_playerSpawn.x * _tileSize, _playerSpawn.y * _tileSize};
	mapCoordinatesToGlobal(pos, pos);
	x = pos.x; y = pos.y;
}

void LevelMap::getMobSpawnPosition(int& x, int& y) const {
	x = _mobSpawn.x;
	y = _mobSpawn.y;
}

bool LevelMap::isOneTile(int x1, int y1, int x2, int y2) const {
	Point tile1, tile2;
	getTileByCoords(x1, y1, tile1.x, tile1.y);
	getTileByCoords(x2, y2, tile2.x, tile2.y);
	return tile1.x == tile2.x && tile1.y == tile2.y;
}

bool LevelMap::isCanGoTo(const Rect& who, int toX, int toY) const {
	Point from1, from2, to;
	getTileByCoords(who.left, who.top, from1.x, from1.y);
	getTileByCoords(who.right, who.bottom, from2.x, from2.y);
	getTileByCoords(toX, toY, to.x, to.y);
	TileType from1Tile = _map[from1.y][from1.x],
			toTile = _map[to.y][to.x];

	//cout << "I wanna go from (" << from.x << ", " << from.y << ", " << fromTile <<
	//		") to (" << to.x << ", " << to.y << ", " << toTile << ")" << endl;

	//int w = who.right - who.left;
	//int h = who.bottom - who.top;

	if(from1.x != from2.x || from1.y != from2.y || ABS(from1.x - to.x) > 1 ||
			ABS(from1.y - to.y) > 1 || ABS(from2.x - to.x) > 1 || ABS(from2.y - to.y) > 1)
		return false;

	switch(from1Tile) {
	case WALL:
	case GATE_CROSSROAD_LEFT:
	case GATE_CROSSROAD_TOP:
	case GATE_CROSSROAD_BOTTOM:
	case GATE_CROSSROAD_RIGHT:
		return false;
	default: break;
	}

	if(from1.x == to.x && from1.y == to.y) {
		return true;
	}

	return isTileHaveEnterAt(toTile, from1.x - to.x, from1.y - to.y);
}

bool LevelMap::isOutOfBounds(int x, int y) const {
	Point pos = {x, y};
	globalCoordinatesToMap(pos, pos);
	return pos.x < 0 || pos.y < 0 || pos.x > _tileSize * _width || pos.y > _tileSize * _height;
}

void LevelMap::setBackToMap(int &x, int &y) const {
	Point pos = {x, y};
	globalCoordinatesToMap(pos, pos);
	if(pos.x < 0) pos.x += _tileSize * _width;
	if(pos.y < 0) pos.y += _tileSize * _height;
	if(pos.x > _tileSize * _width) pos.x -= _tileSize * _width;
	if(pos.y > _tileSize * _height) pos.y -= _tileSize * _height;
	mapCoordinatesToGlobal(pos, pos);
	x = pos.x; y = pos.y;
}

void LevelMap::draw() const {
	if(!_map) return;

	for(int y = -_yOffset / _tileSize;
		y < (-_yOffset + _screenHeight) / _tileSize + 1 && y < _height; y++)
		for(int x = -_xOffset / _tileSize;
			x < (-_xOffset + _screenWidth) / _tileSize + 1 && x < _width; x++) {
			_tiles->drawTile(_map[y][x], _xOffset + x * _tileSize, _yOffset + y * _tileSize,
							 0, _tileSize, _tileSize);
		}
}

bool LevelMap::isTileHaveEnterAt(const TileType &tile, int x, int y) {
	switch(tile) {
	case TOP_LEFT:
	case CL_TOP_LEFT:
		return (x == 0 && y == 1) || (x == 1 && y == 0);
	case T_DOWN:
	case CL_T_DOWN:
		return (x == 0 && y == 1) || (y == 0 && (x == -1 || x == 1));
	case TOP_RIGHT:
	case CL_TOP_RIGHT:
		return (x == 0 && y == 1) || (x == -1 && y == 0);
	case ROAD_LEFT:
		return y == 0 && (x == -1 || x == 1);
	case ROAD_UP:
		return x == 0 && (y == -1 || y == 1);
	case T_LEFT:
	case CL_T_LEFT:
		return (x == 1 && y == 0) || (x == 0 && (y == -1 || y == 1));
	case CROSSROAD:
	case CL_CROSSROAD:
	case CL_CROSSROAD_LEFT:
	case CL_CROSSROAD_TOP:
	case CL_CROSSROAD_BOTTOM:
	case CL_CROSSROAD_RIGHT:
		return (x == 0 && (y == -1 || y == 1)) || (y == 0 && (x == -1 || x == 1));
	case T_RIGHT:
	case CL_T_RIGHT:
		return (x == -1 && y == 0) || (x == 0 && (y == -1 || y == 1));
	case BOTTOM_LEFT:
	case CL_BOTTOM_LEFT:
		return (x == 0 && y == -1) || (x == 1 && y == 0);
	case T_UP:
	case CL_T_UP:
		return (x == 0 && y == -1) || (y == 0 && (x == -1 || x == 1));
	case BOTTOM_RIGHT:
	case CL_BOTTOM_RIGHT:
		return (x == 0 && y == -1) || (x == -1 && y == 0);
	default:
		return false;
	}
}
