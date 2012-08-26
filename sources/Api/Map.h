#ifndef MAP_H
#define MAP_H

class Map {
public:
	virtual ~Map() = 0;

	virtual void fillScreen(int width, int height, int minTileSize = 16) = 0;
	virtual void setCenter(int x, int y) = 0;
	virtual int getTileSize() { return _tileSize; }

	virtual void draw() const = 0;

protected:
	int _tileSize;
	int _xOffset, _yOffset;
	int _screenWidth, _screenHeight;

	virtual void setTileSize(int size) = 0;
};

#endif // MAP_H
