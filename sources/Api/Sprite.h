#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"

class Sprite
{
public:
	static Sprite * create(const Texture *texture, float x, float y, float z);

	~Sprite();
	void draw();

	int getX() const;
	int getY() const;
	void getPosition(int &x, int &y) const;

	void setPosition(int x, int y);
	void setScale(float scale);

protected:
	float _x, _y, _z;
	float _scale;
	const Texture *_texture;

	Sprite(float x, float y, float z);
};

#endif // SPRITE_H
