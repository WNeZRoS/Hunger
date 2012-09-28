#include "PathFinder.h"
#include "LevelMap.h"
#include "Api/Type/Float.h"

PathFinder::PathFinder(LevelMap *map) {
	_map = map;
	_roadPos = _map->getOne() / 2.0f;
}

PathFinder::~PathFinder() {

}

bool PathFinder::correctMotion(Point &from, const Point &to) {
	Point_i fromTilePos;
	Point roadPos;
	this->getPositions(from, fromTilePos, roadPos);

	//Log::Debug << "from tile " << _map->getTileByCoords(fromTilePos).type() << " " << fromTilePos << " road pos " << roadPos;

	//if(!getTileByCoords(fromTilePos).haveRoadAt(roadPos, _tileSize)) return false;

	Point newPos = to;
	_map->turnToBounds(newPos);

	Point_i newTilePos;
	Point newRoadPos;
	this->getPositions(newPos, newTilePos, newRoadPos);

	//Log::Debug << "new tile " << _map->getTileByCoords(newTilePos).type() << " " << newTilePos << " road pos " << newRoadPos;

	if(this->haveRoad(_map->getTileByCoords(newTilePos), newRoadPos)) {
		from = newPos;
		return true;
	}

	roadPos = newRoadPos;
	if(this->setToRoad(_map->getTileByCoords(newTilePos), roadPos, newTilePos - fromTilePos)) {
		//Log::Debug << "delta road " << (roadPos.x - newRoadPos.x) << " " << (roadPos.y - newRoadPos.y);
		Point newPos2 = newPos + roadPos - newRoadPos;
		if(newPos != newPos2 && newPos2 != from) {
			from = newPos2;
			return true;
		}
		return false;
	}

	return false;
}

void PathFinder::getPositions(const Point& pos, Point_i& tilePos, Point& roadPos) {
	_map->globalToMap(pos, tilePos);
	_map->mapToGlobal(tilePos, roadPos);
	roadPos = pos - roadPos;
}

bool PathFinder::haveRoad(const Tile& tile, const Point& pos) {
	if(!tile.isRoad()) return false;

	if(pos.x == _roadPos) {
		return (tile.isRoadDown() && pos.y >= _roadPos)
				|| (tile.isRoadUp() && pos.y <= _roadPos);
	} else if(pos.y == _roadPos) {
		return (tile.isRoadLeft() && pos.x <= _roadPos)
				|| (tile.isRoadRight() && pos.x >= _roadPos);
	}

	return false;
}

bool PathFinder::setToRoad(const Tile& tile, Point& pos, const Point_i& dir) {
	if(this->haveRoad(tile, pos)) return true;

	float dx = (pos.x - _roadPos).abs().f(), dy = (pos.y - _roadPos).abs().f();
	bool hx = this->haveRoad(tile, Point(_roadPos, pos.y));
	bool hy = this->haveRoad(tile, Point(pos.x, _roadPos));

	if(hx) {
		if(hy) {
			if(dx < dy) pos.x = _roadPos;
			else pos.y = _roadPos;
		} else pos.x = _roadPos;
	} else if(hy) pos.y = _roadPos;

	return hx || hy;
}

bool PathFinder::findPath(const Point &from, const Point &to, Array<PathElement> &path) {
	return this->findPath(from, to, Point::NaN, path);
}

bool PathFinder::findPath(const Point &from, const Point &to, const Point &noCross, Array<PathElement> &path) {
	Point_i tileFrom, tileTo, tileNoCross(-1, -1);
	_map->globalToMap(from, tileFrom);
	_map->globalToMap(to, tileTo);
	if(!noCross.nan()) _map->globalToMap(noCross, tileNoCross);

	TwoDimArray<PathStep>::T steps = TwoDimArray<PathStep>::array(_map->_height, _map->_width);
	steps = -1;

#warning TODO X cache

	unsigned int size = this->findPath(Point_i::ONE * -1, tileFrom, tileTo, tileNoCross, steps, 0, -1);
	if(size != 0 && size != static_cast<unsigned int>(-1)) {
		path.rebuild(size + 1);
		return this->buildPath(tileTo, steps, path, size);
	}

	return false;
}

unsigned int PathFinder::findPath(const Point_i& parent, const Point_i &pfrom, const Point_i &to, const Point_i &noCross,
								  TwoDimArray<PathStep>::T &steps, unsigned int step, unsigned int minSteps) {
	Point_i from = pfrom;
	_map->turnToBounds(from);

	if(step >= minSteps) return -1;
	if(steps[from.y][from.x].step <= step) return -1;
	if(!_map->getTileByCoords(from).isRoad()) return -1;
	if(from == noCross) return -1;

	if(from == to) {
		steps[from.y][from.x].step = step;
		steps[from.y][from.x].pos = parent;
		return step;
	}

	unsigned int prevState = steps[from.y][from.x].step;
	steps[from.y][from.x].step = step;

	unsigned int min = -1;
	Point_i direction = to - from;

	bool second = false;
	do {
		if(_map->getTileByCoords(from).isRoadUp() && direction.y <= 0) {
			unsigned int size = this->findPath(from, from - Point_i::iONE, to, noCross, steps, step + 1, minSteps);
			if(size < min) min = size;
			if(min < minSteps) minSteps = min;
		}

		if(_map->getTileByCoords(from).isRoadDown() && direction.y > 0) {
			unsigned int size = this->findPath(from, from + Point_i::iONE, to, noCross, steps, step + 1, minSteps);
			if(size < min) min = size;
			if(min < minSteps) minSteps = min;
		}

		if(_map->getTileByCoords(from).isRoadLeft() && direction.x >= 0) {
			unsigned int size = this->findPath(from, from - Point_i::ONE, to, noCross, steps, step + 1, minSteps);
			if(size < min) min = size;
			if(min < minSteps) minSteps = min;
		}

		if(_map->getTileByCoords(from).isRoadRight() && direction.x < 0) {
			unsigned int size = this->findPath(from, from + Point_i::ONE, to, noCross, steps, step + 1, minSteps);
			if(size < min) min = size;
			if(min < minSteps) minSteps = min;
		}

		direction *= -1;
		if(direction.x == 0) direction.x = 1;
		if(direction.y == 0) direction.y = 1;
		second = !second;
	} while(second);

	if(min == static_cast<unsigned int>(-1))
		steps[from.y][from.x].step = prevState;
	else {
		steps[from.y][from.x].pos = parent;
	}

	return min;
}

bool PathFinder::buildPath(const Point_i &pos, const TwoDimArray<PathStep>::T &steps, Array<PathElement> &path, unsigned int step) {
	if(steps[pos.y][pos.x].step != step) return false;

	Point element;
	_map->mapToGlobal(pos, element);
	path[step].pos = element;

	if(step > 0) return this->buildPath(steps[pos.y][pos.x].pos, steps, path, step - 1);
	return true;
}

PathFinder::PathStep& PathFinder::PathStep::operator =(unsigned int step) {
	this->step = step;
	return *this;
}
