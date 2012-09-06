#include "Sprite.h"
#include "Render.h"
#include "TextureManager.h"
#include "compatibility.h"

SpriteBase::SpriteBase(float x, float y, float z) {
	setPosition(x, y);
	setScale(1.0f);
	_z = z;
}

SpriteBase::~SpriteBase() {

}

void SpriteBase::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

void SpriteBase::setScale(float scale) {
	_scale = scale;
}

void SpriteBase::getPosition(int &x, int &y) const {
	x = _x;
	y = _y;
}

int SpriteBase::getX() const {
	return _x;
}

int SpriteBase::getY() const {
	return _y;
}

// Sprite class

Sprite * Sprite::create(const Texture::Name texture, float x, float y, float z) {
	if(!texture) return NULL;
	Sprite *sprite = new Sprite(x, y, z);
	sprite->_texture = LOAD_TEXTURE(texture);
	if(!sprite->_texture) {
		delete sprite;
		return NULL;
	}
	return sprite;
}

Sprite::Sprite(float x, float y, float z) : SpriteBase(x, y, z) {

}

Sprite::~Sprite() {
	if(_texture) UNLOAD_TEXTURE(_texture);
}

void Sprite::draw() {
	_texture->activate();
	Render::getPainter()->rectx(_x, _y, _x + _scale, _y + _scale, _z);
}
