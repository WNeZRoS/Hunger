#include "Sprite.h"
#include "Render.h"

Sprite::Sprite(const Texture *texture, int x, int y, int z) {
	setPosition(x, y);
	setScale(1.0f);
	_z = z;
	_texture = texture;
}

Sprite::Sprite(int x, int y, int z) {
	setPosition(x, y);
	setScale(1.0f);
	_z = z;
}

Sprite::~Sprite() {

}

void Sprite::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

void Sprite::setScale(float scale) {
	_scale = scale;
}

void Sprite::getPosition(int &x, int &y) const {
	x = _x;
	y = _y;
}

int Sprite::getX() const {
	return _x;
}

int Sprite::getY() const {
	return _y;
}

void Sprite::draw() {
	_texture->activate();
	Render::getPainter()->rectx(_x, _y, _x + _scale, _y + _scale, _z);
}
