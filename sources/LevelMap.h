#ifndef LEVELMAP_H
#define LEVELMAP_H

#include "Api/Texture.h"
#include "Api/TextureAtlas.h"
#include "Api/Map.h"

struct Point {
	int x, y;
};

struct Rect {
	int top, left, bottom, right;
};

class LevelMap : public Map {
public:
	// (  )(|=)(=Г)(=|)
	// (==)(|-)(++)(-|)
	// (||)(|_)(_L)(_|)
	enum TileType {
		WALL = 0, TOP_LEFT, T_DOWN, TOP_RIGHT, CL_TOP_LEFT, CL_T_DOWN, CL_TOP_RIGHT, FREE_1,
		ROAD_LEFT, T_LEFT, CROSSROAD, T_RIGHT, CL_T_LEFT, CL_CROSSROAD, CL_T_RIGHT, FREE_2,
		ROAD_UP, BOTTOM_LEFT, T_UP, BOTTOM_RIGHT, CL_BOTTOM_LEFT, CL_T_UP, CL_BOTTOM_RIGHT, FREE_3,
		CL_CROSSROAD_LEFT, CL_CROSSROAD_TOP, CL_CROSSROAD_BOTTOM, CL_CROSSROAD_RIGHT,
		GATE_CROSSROAD_LEFT, GATE_CROSSROAD_TOP, GATE_CROSSROAD_BOTTOM, GATE_CROSSROAD_RIGHT
	};

	LevelMap(const TileType **map, int width, int height, const Texture *tiles,
			 const Point& playerSpawn, const Point& mobSpawn);
	~LevelMap();

	void fillScreen(int width, int height, int minTileSize = 16);
	void setCenter(int x, int y);

	static LevelMap * load(const char * filename, const Texture *tiles);

	void draw() const;

	const TileType& getTileByCoords(int x, int y) const;
	void getTileByCoords(int x, int y, int& tx, int& ty) const;

	void getPlayerSpawnPosition(int& x, int& y) const;
	void getMobSpawnPosition(int& x, int& y) const;
	bool isOneTile(int x1, int y1, int x2, int y2) const;
	bool isCanGoTo(const Rect& who, int toX, int toY) const;
	bool isOutOfBounds(int x, int y) const;
	void setBackToMap(int& x, int& y) const;

	void globalCoordinatesToMap(const Point& coord, Point& map) const;
	void globalCoordinatesToScreen(const Point& global, Point& screen) const;
	void mapCoordinatesToGlobal(const Point& map, Point& coord) const;

private:
	const TileType ** _map;
	int _width, _height;
	Point _playerSpawn, _mobSpawn;
	TextureAtlas * _tiles;

	void setTileSize(int size);

	static bool isTileHaveEnterAt(const TileType& tile, int x, int y);
};

#endif // LEVELMAP_H
