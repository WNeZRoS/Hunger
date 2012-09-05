#ifndef LEVELMAP_H
#define LEVELMAP_H

#include "Api/Texture.h"
#include "Api/TextureAtlas.h"
#include "Api/Map.h"
#include <fstream>

struct Point
{
	int x, y;
	bool operator == (const Point& p) { return x == p.x && y == p.y; }
	bool operator != (const Point& p) { return !(*this == p); }
};

struct Rect
{
	int top, left, bottom, right;
};

class LevelMap : public Map
{
public:
	class Tile
	{
	public:
		// (  )(|=)(=Г)(=|)
		// (==)(|-)(++)(-|)
		// (||)(|_)(_L)(_|)
		enum TileType
		{
			WALL = 0, TOP_LEFT, T_DOWN, TOP_RIGHT, CL_TOP_LEFT, CL_T_DOWN, CL_TOP_RIGHT, FREE_1,
			ROAD_LEFT, T_LEFT, CROSSROAD, T_RIGHT, CL_T_LEFT, CL_CROSSROAD, CL_T_RIGHT, FREE_2,
			ROAD_UP, BOTTOM_LEFT, T_UP, BOTTOM_RIGHT, CL_BOTTOM_LEFT, CL_T_UP, CL_BOTTOM_RIGHT, FREE_3,
			CL_CROSSROAD_LEFT, CL_CROSSROAD_TOP, CL_CROSSROAD_BOTTOM, CL_CROSSROAD_RIGHT,
			GATE_CROSSROAD_LEFT, GATE_CROSSROAD_TOP, GATE_CROSSROAD_BOTTOM, GATE_CROSSROAD_RIGHT
		};

		Tile();
		Tile(TileType type);
		~Tile();

		const TileType& type() const;
		bool haveRoadAt(const Point& pos, int tileSize) const;
		bool canSetToRoad(Point& pos, int tileSize) const;

		friend std::istream& operator >> (std::istream& in, Tile& t);
	private:
		TileType _type;

		bool isCenterUp(const Point& pos, int halfSize) const;
		bool isCenterBottom(const Point& pos, int halfSize) const;
		bool isLeftCenter(const Point& pos, int halfSize) const;
		bool isRightCenter(const Point& pos, int halfSize) const;
	};

	LevelMap(const Tile **map, int width, int height, const Texture *tiles,
			 const Point& playerSpawn, const Point& mobSpawn);
	~LevelMap();

	void fillScreen(int width, int height, int minTileSize = 16);
	void setCenter(int x, int y);

	static LevelMap * load(const char *filename, const Texture *tiles);

	void draw() const;

	const Tile& getTileByCoords(const Point& coord) const;
	void getPlayerSpawnPosition(int& x, int& y) const;
	void getMobSpawnPosition(int& x, int& y) const;

	void globalCoordinatesToMap(const Point& coord, Point& map) const;
	void globalCoordinatesToScreen(const Point& global, Point& screen) const;
	void mapCoordinatesToGlobal(const Point& map, Point& coord) const;

	bool moveInDirection(Point& from, const Point& dir, float speed = 1.0f) const;
	int distance(Point tile1, const Point& tile2) const;
private:
	const Tile **_map;
	int _width, _height;
	Point _playerSpawn, _mobSpawn;
	TextureAtlas *_tiles;

	void setTileSize(int size);

	void getPositions(const Point& pos, Point& tilePos, Point& roadPos) const;
	//bool isOneTile(int x1, int y1, int x2, int y2) const;
	//bool isCanGoTo(Point& from, const Point& to) const;
	//bool isOutOfBounds(int x, int y) const;
	//void setBackToMap(int& x, int& y) const;
	//static bool isTileHaveEnterAt(const TileType& tile, const Point& coord);
};

#endif // LEVELMAP_H
