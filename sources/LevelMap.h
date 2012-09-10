#ifndef LEVELMAP_H
#define LEVELMAP_H

#include "Api/Api.h"

struct Rect
{
	int top, left, bottom, right;
};

// TODO: add food (tablet, apple, cake) spawn position to .map

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
		bool isRoad() const;
		bool canSetToRoad(Point& pos, int tileSize) const;

		friend std::istream& operator >> (std::istream& in, Tile& t);
	private:
		TileType _type;

		bool isCenterUp(const Point& pos, float halfSize) const;
		bool isCenterBottom(const Point& pos, float halfSize) const;
		bool isLeftCenter(const Point& pos, float halfSize) const;
		bool isRightCenter(const Point& pos, float halfSize) const;
	};

	LevelMap(const Tile **map, int width, int height, const Texture::Name tiles,
			 const Point_i& playerSpawn, const Point_i& mobSpawn, Array<Point_i>& foodSpawn);
	~LevelMap();

	static LevelMap * load(const char *filename, const Texture::Name tiles);

	void draw() const;

	Point getPlayerSpawnPosition() const;
	Point getMobSpawnPosition() const;
	Array<Point> getFoodSpawns() const;
	void getRoads(Array<Point>& roads) const;

	float getOne() const;
	void globalCoordinatesToMap(const Point& coord, Point_i& map) const;
	void globalCoordinatesToScreen(const Point& global, Point& screen) const;
	void mapCoordinatesToGlobal(const Point_i& map, Point& coord) const;

	bool moveInDirection(Point& from, const Point& dir, float speed = 1.0f) const;
	float distance(Point tile1, const Point& tile2) const;
private:
	const Tile **_map;
	int _width, _height;
	Point_i _playerSpawn, _mobSpawn;
	Array<Point_i> _foodSpawn;
	TextureAtlas *_tiles;

	void fillScreen(int width, int height, int minTileSize);
	void setCenter(float x, float y);
	void setTileSize(int size);

	const Tile& getTileByCoords(const Point_i& coord) const;
	void getPositions(const Point& pos, Point_i& tilePos, Point& roadPos) const;
};

#endif // LEVELMAP_H
