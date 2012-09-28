#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Api/Type/Array.h"
#include "Api/Type/Point.h"

class Tile;
class LevelMap;

class PathFinder
{
public:
	struct PathElement {
		Point pos;
	};

	PathFinder(LevelMap *map);
	~PathFinder();

	// Move correction
	bool correctMotion(Point& from, const Point& to);

	// Path finding
	bool findPath(const Point& from, const Point& to, Array<PathElement>& path);
	bool findPath(const Point& from, const Point& to, const Point& noCross, Array<PathElement>& path);

private:
	struct PathStep {
		unsigned int step;
		Point_i pos;

		PathStep& operator =(unsigned int step);
	};

	LevelMap *_map;
	float _roadPos;

	// Move correction
	void getPositions(const Point& pos, Point_i& tilePos, Point& roadPos);
	bool haveRoad(const Tile& tile, const Point& pos);
	bool setToRoad(const Tile& tile, Point& pos, const Point_i& dir);

	// Path finding
	unsigned int findPath(const Point_i& parent, const Point_i& from, const Point_i& to, const Point_i& noCross,
						  TwoDimArray<PathStep>::T& steps, unsigned int step, unsigned int minSteps);
	bool buildPath(const Point_i& pos, const TwoDimArray<PathStep>::T& steps, Array<PathElement>& path, unsigned int step);
};

#endif // PATHFINDER_H
