#include "Sprite.h"
#include "Render.h"
#include "TextureManager.h"
#include "compatibility.h"

Sprite * Sprite::create(const Texture *texture, float x, float y, float z) {
	if(!texture) return NULL;
	Sprite *sprite = new Sprite(x, y, z);
	sprite->_texture = texture;
	return sprite;
}

Sprite::Sprite(float x, float y, float z) {
	setPosition(x, y);
	setScale(1.0f);
	_z = z;
}

Sprite::~Sprite() {
	TextureManager::instance()->unload(_texture);
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
