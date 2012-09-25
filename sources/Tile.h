#ifndef TILE_H
#define TILE_H

#include <fstream>

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

	//bool canSetToRoad(Point& pos, int tileSize) const;
	//bool haveRoadAt(const Point& pos, int tileSize) const;

	bool isRoad() const;
	bool isClear() const;

	bool isRoadUp() const;
	bool isRoadDown() const;
	bool isRoadLeft() const;
	bool isRoadRight() const;

	bool isGate() const;
	void openGate();
	void closeGate();

	friend std::istream& operator >> (std::istream& in, Tile& t);
private:
	TileType _type;
	bool _isRoad, _isClear, _isGate, _isRoadUp, _isRoadDown, _isRoadLeft, _isRoadRight;

	void setType(TileType type);

	//bool isCenterUp(const Point& pos, float halfSize) const;
	//bool isCenterBottom(const Point& pos, float halfSize) const;
	//bool isLeftCenter(const Point& pos, float halfSize) const;
	//bool isRightCenter(const Point& pos, float halfSize) const;
};

#endif // TILE_H
