#include "TileSprite.h"
#include "compatibility.h"

TileSprite * TileSprite::create(const TextureAtlas::Loader atlasLoader, float x, float y, float z) {
	TextureAtlas * atlas = atlasLoader.load();
	if(!atlas) return NULL;
	return new TileSprite(atlas, x, y, z);
}

TileSprite::TileSprite(const TextureAtlas *atlas, float x, float y, float z) : SpriteBase(x, y, z) {
	_texture = atlas;
	_tileId = 0;
	_animation.frames = 0;
	_animation.framesCount = 0;
	_lastFrameTime = 0;
	_currentFrameId = 0;
}

TileSprite::~TileSprite() {
	_texture->unload();
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
