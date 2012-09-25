#include "Tile.h"

Tile::Tile() {
	setType(WALL);
}

Tile::Tile(TileType type) {
	setType(type);
}

Tile::~Tile() {

}

const Tile::TileType& Tile::type() const {
	return _type;
}

void Tile::setType(TileType type) {
	_type = type;

	_isRoad = _type != WALL && _type != FREE_1 && _type != FREE_2 && _type != FREE_3;

	if(_isRoad) {
		_isRoadUp = _type == ROAD_UP || _type == T_UP || _type == BOTTOM_LEFT || _type == BOTTOM_RIGHT
				|| _type == T_LEFT || _type == T_RIGHT || _type == CROSSROAD || _type == CL_T_UP
				|| _type == CL_T_LEFT || _type == CL_CROSSROAD || _type == CL_T_RIGHT
				|| _type == CL_BOTTOM_LEFT || _type == CL_BOTTOM_RIGHT || _type == CL_CROSSROAD_LEFT
				|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT
				|| _type == GATE_CROSSROAD_LEFT || _type == GATE_CROSSROAD_RIGHT || _type == GATE_CROSSROAD_BOTTOM;

		_isRoadDown = _type == ROAD_UP || _type == TOP_LEFT || _type == TOP_RIGHT || _type == T_DOWN
				|| _type == T_LEFT || _type == T_RIGHT || _type == CROSSROAD || _type == CL_TOP_LEFT
				|| _type == CL_TOP_RIGHT || _type == CL_T_DOWN || _type == CL_T_LEFT
				|| _type == CL_T_RIGHT || _type == CL_CROSSROAD || _type == CL_CROSSROAD_LEFT
				|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT
				|| _type == GATE_CROSSROAD_LEFT || _type == GATE_CROSSROAD_RIGHT || _type == GATE_CROSSROAD_TOP;

		_isRoadLeft = _type == ROAD_LEFT || _type == TOP_RIGHT || _type == T_DOWN || _type == BOTTOM_RIGHT
				|| _type == T_UP || _type == T_RIGHT || _type == CROSSROAD || _type == CL_TOP_RIGHT
				|| _type == CL_T_DOWN || _type == CL_BOTTOM_RIGHT || _type == CL_T_UP
				|| _type == CL_T_RIGHT || _type == CL_CROSSROAD || _type == CL_CROSSROAD_LEFT
				|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT
				|| _type == GATE_CROSSROAD_TOP || _type == GATE_CROSSROAD_RIGHT || _type == GATE_CROSSROAD_BOTTOM;

		_isRoadRight = _type == ROAD_LEFT  || _type == TOP_LEFT || _type == T_DOWN || _type == BOTTOM_LEFT
				|| _type == T_UP || _type == T_LEFT ||  _type == CROSSROAD || _type == CL_TOP_LEFT
				|| _type == CL_T_DOWN || _type == CL_BOTTOM_LEFT || _type == CL_T_UP
				|| _type == CL_T_LEFT || _type == CL_CROSSROAD || _type == CL_CROSSROAD_LEFT
				|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT
				|| _type == GATE_CROSSROAD_LEFT || _type == GATE_CROSSROAD_TOP || _type == GATE_CROSSROAD_BOTTOM;
	} else _isRoadUp = _isRoadDown = _isRoadLeft = _isRoadRight = false;

	_isGate = _type == GATE_CROSSROAD_LEFT || _type == GATE_CROSSROAD_TOP
			|| _type == GATE_CROSSROAD_RIGHT || _type == GATE_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_LEFT
			|| _type == CL_CROSSROAD_TOP || _type == CL_CROSSROAD_BOTTOM || _type == CL_CROSSROAD_RIGHT;

	_isClear = _type == CL_TOP_LEFT || _type == CL_T_DOWN || _type == CL_TOP_RIGHT || _type == CL_T_LEFT
			|| _type == CL_CROSSROAD || _type == CL_T_RIGHT || _type == CL_BOTTOM_LEFT
			|| _type == CL_T_UP || _type == CL_BOTTOM_RIGHT || _isGate;
}

std::istream& operator >>(std::istream& in, Tile& t) {
	int x;
	in >> x;
	t.setType(static_cast<Tile::TileType>(x));
	return in;
}

/*bool Tile::haveRoadAt(const Point& pos, int tileSize) const {
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
}*/

bool Tile::isRoad() const {
	return _isRoad;
}

bool Tile::isClear() const {
	return _isClear;
}

bool Tile::isRoadUp() const {
	return _isRoadUp;
}

bool Tile::isRoadDown() const {
	return _isRoadDown;
}

bool Tile::isRoadLeft() const {
	return _isRoadLeft;
}

bool Tile::isRoadRight() const {
	return _isRoadRight;
}

/*bool Tile::canSetToRoad(Point& pos, int tileSize) const {
	if(this->haveRoadAt(pos, tileSize)) return true;
	float halfSize = 0.5f * tileSize;
	float dx = mabs(pos.x - halfSize), dy = mabs(pos.y - halfSize);
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

bool Tile::isCenterUp(const Point& pos, float halfSize) const {
	return equal(pos.x, halfSize) && pos.y <= halfSize;
}

bool Tile::isCenterBottom(const Point& pos, float halfSize) const {
	return equal(pos.x, halfSize) && pos.y >= halfSize;
}

bool Tile::isLeftCenter(const Point& pos, float halfSize) const {
	return pos.x <= halfSize && equal(pos.y, halfSize);
}

bool Tile::isRightCenter(const Point& pos, float halfSize) const {
	return pos.x >= halfSize && equal(pos.y, halfSize);
}*/

bool Tile::isGate() const {
	return _isGate;
}

void Tile::openGate() {
	switch(_type) {
	case GATE_CROSSROAD_LEFT: setType(CL_CROSSROAD_LEFT); break;
	case GATE_CROSSROAD_TOP: setType(CL_CROSSROAD_TOP); break;
	case GATE_CROSSROAD_RIGHT: setType(CL_CROSSROAD_RIGHT); break;
	case GATE_CROSSROAD_BOTTOM: setType(CL_CROSSROAD_BOTTOM); break;
	default: break;
	}
}

void Tile::closeGate() {
	switch(_type) {
	case CL_CROSSROAD_LEFT: setType(GATE_CROSSROAD_LEFT); break;
	case CL_CROSSROAD_TOP: setType(GATE_CROSSROAD_TOP); break;
	case CL_CROSSROAD_RIGHT: setType(GATE_CROSSROAD_RIGHT); break;
	case CL_CROSSROAD_BOTTOM: setType(GATE_CROSSROAD_BOTTOM); break;
	default: break;
	}
}
