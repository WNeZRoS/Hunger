#ifndef MAP_H
#define MAP_H

class Map
{
public:
	virtual ~Map() = 0;

	virtual float getOne() const { return 1.0f; }
	virtual int getTileSize() { return _tileSize; }
	virtual void draw() const = 0;

	friend class World;
protected:
	int _tileSize;
	float _xOffset, _yOffset;
	int _screenWidth, _screenHeight;

	virtual void fillScreen(int width, int height, int minTileSize = 8) = 0;
	virtual void setCenter(float x, float y) = 0;
	virtual void setTileSize(int size) = 0;
};

#endif // MAP_H
