#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"

class Sprite {
public:
	Sprite(const Texture * texture, int x, int y, int z);
	~Sprite();
	void draw();

	int getX() const;
	int getY() const;
	void getPosition(int &x, int &y) const;

	void setPosition(int x, int y);
	void setScale(float scale);

protected:
	int _x, _y, _z;
	float _scale;
	const Texture * _texture;

	Sprite(int x, int y, int z);
};

#endif // SPRITE_H
