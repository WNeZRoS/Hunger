#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"

class SpriteBase
{
public:
	virtual ~SpriteBase() = 0;
	virtual void draw() = 0;

	float getX() const;
	float getY() const;
	void getPosition(float &x, float &y) const;

	void setPosition(float x, float y);
	void setScale(float scale);

protected:
	float _x, _y, _z;
	float _scale;

	SpriteBase(float x, float y, float z);
};

class Sprite : public SpriteBase
{
public:
	static Sprite * create(const Texture::Name texture, float x, float y, float z);

	~Sprite();
	void draw();

protected:
	const Texture *_texture;

	Sprite(float x, float y, float z);
};

#endif // SPRITE_H
