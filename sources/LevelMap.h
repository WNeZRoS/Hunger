#ifndef LEVELMAP_H
#define LEVELMAP_H

#include "Api/Api.h"
#include "Tile.h"
#include "PathFinder.h"

class LevelMap : public Map
{
	friend class PathFinder;

public:
	LevelMap(const Tile **map, int width, int height, const Texture::Name tiles,
			 const Point_i& playerSpawn, const Point_i& mobSpawn, Array<Point_i>& foodSpawn);
	~LevelMap();

	static LevelMap * load(const char *filename, const Texture::Name tiles);

	void draw() const;

	float getOne() const;
	void globalToScreen(const Point& global, Point& screen) const;

	Point getPlayerSpawnPosition() const;
	Point getMobSpawnPosition() const;
	Array<Point> getFoodSpawns() const;
	void getRoads(Array<Point>& roads) const;

	void openGates();
	void closeGates();

	bool isRoad(const Point& globalPos) const;
	void turnToBounds(Point& globalPos) const;

	PathFinder * getPathFinder() const;

private:
	Tile **_map;
	int _width, _height;
	Point_i _playerSpawn, _mobSpawn;
	Array<Point_i> _foodSpawn;
	TextureAtlas *_tiles;
	PathFinder *_pathFinder;

	void fillScreen(int width, int height, int minTileSize);
	void setCenter(float x, float y);
	void setTileSize(int size);

	void mapToGlobal(const Point_i& map, Point& coord) const;
	void globalToMap(const Point& coord, Point_i& map) const;

	void turnToBounds(Point_i& mapPos) const;
	const Tile& getTileByCoords(const Point_i& tileCoord) const;

};

#endif // LEVELMAP_H
