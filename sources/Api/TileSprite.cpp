#include "TileSprite.h"
#include "compatibility.h"

TileSprite * TileSprite::create(const TextureAtlas *atlas, float x, float y, float z) {
	if(!atlas) return NULL;
	return new TileSprite(atlas, x, y, z);
}

TileSprite::TileSprite(const TextureAtlas * atlas, float x, float y, float z) : Sprite(x, y, z) {
	_texture = atlas;
	_tileId = 0;
	_animation.frames = 0;
	_animation.framesCount = 0;
	_lastFrameTime = 0;
	_currentFrameId = 0;
}

TileSprite::~TileSprite() {
	delete _texture;
}

int TileSprite::getTileId() const {
	return _tileId;
}

void TileSprite::setTileId(int id) {
	_tileId = id;
}

void TileSprite::animate(const Animation& animation) {
	_animation = animation;
	_currentFrameId = 0;
	_lastFrameTime = getCurrentTime();
}

void TileSprite::replaceAnimation(const Animation& animation) {
	_animation = animation;
	if(_lastFrameTime == 0) _lastFrameTime = getCurrentTime();
}

void TileSprite::clearAnimation() {
	_animation.framesCount = 0;
	_currentFrameId = 0;
}

void TileSprite::draw() {
	if(_animation.frames && _animation.framesCount > 0) {
		if(_lastFrameTime + _animation.frames[_currentFrameId].delay <= getCurrentTime()) {
			_tileId = _animation.frames[_currentFrameId].tileId;
			if(_animation.moveSpeed > 0) {
				if(ABS(_animation.moveTo.x - _x) < _animation.moveSpeed)
					_x = _animation.moveTo.x;
				else if(_animation.moveTo.x - _x != 0)
					_x += (_animation.moveTo.x - _x < 0 ? -1 : 1) * _animation.moveSpeed;

				if(ABS(_animation.moveTo.y - _y) < _animation.moveSpeed)
					_y = _animation.moveTo.y;
				else if(_animation.moveTo.y - _y != 0)
					_y += (_animation.moveTo.y - _y < 0 ? -1 : 1) * _animation.moveSpeed;

				if(_animation.moveTo.x == _x && _animation.moveTo.y == _y)
					_animation.moveSpeed = 0;
			}
			_currentFrameId++;
			_lastFrameTime = getCurrentTime();
			if(_currentFrameId >= _animation.framesCount) {
				if(_animation.loop > 0) {
					_currentFrameId = 0;
					_animation.loop--;
				} else clearAnimation();
			}
		}
	}

	_texture->drawTile(_tileId, _x, _y, _z, _scale, _scale);
}
