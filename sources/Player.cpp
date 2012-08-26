#include "Player.h"

Player::Player(const LevelMap *map, const Texture *texture) {
	_z = 10;
	_map = const_cast<LevelMap*>(map);
	_phisTileSize = _map->getTileSize()/1.5;
	_tileSize = _map->getTileSize();

	_map->getPlayerSpawnPosition(_x, _y);
	_x += _tileSize / 2;
	_y += _tileSize / 2;

	Point pos = {_x - _tileSize / 2, _y - _tileSize / 2};
	_map->globalCoordinatesToScreen(pos, pos);
	_sprite = new TileSprite(texture->toAtlas(4, 4), pos.x, pos.y, _z);
	_sprite->setScale(_map->getTileSize());

	_moveAnimationLeft.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationLeft.framesCount = 2;
	_moveAnimationLeft.loop = 1;
	_moveAnimationLeft.frames[0].tileId = 0; _moveAnimationLeft.frames[0].delay = 150;
	_moveAnimationLeft.frames[1].tileId = 4; _moveAnimationLeft.frames[1].delay = 150;
	_moveAnimationLeft.moveSpeed = 0;

	_moveAnimationRight.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationRight.framesCount = 2;
	_moveAnimationRight.loop = 1;
	_moveAnimationRight.frames[0].tileId = 1; _moveAnimationRight.frames[0].delay = 150;
	_moveAnimationRight.frames[1].tileId = 5; _moveAnimationRight.frames[1].delay = 150;
	_moveAnimationRight.moveSpeed = 0;

	_moveAnimationUp.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationUp.framesCount = 2;
	_moveAnimationUp.loop = 1;
	_moveAnimationUp.frames[0].tileId = 2; _moveAnimationUp.frames[0].delay = 150;
	_moveAnimationUp.frames[1].tileId = 6; _moveAnimationUp.frames[1].delay = 150;
	_moveAnimationUp.moveSpeed = 0;

	_moveAnimationDown.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationDown.framesCount = 2;
	_moveAnimationDown.loop = 1;
	_moveAnimationDown.frames[0].tileId = 3; _moveAnimationDown.frames[0].delay = 150;
	_moveAnimationDown.frames[1].tileId = 7; _moveAnimationDown.frames[1].delay = 150;
	_moveAnimationDown.moveSpeed = 0;
}

Player::~Player() {
	delete _sprite;
	delete [] _moveAnimationLeft.frames;
	delete [] _moveAnimationRight.frames;
	delete [] _moveAnimationUp.frames;
	delete [] _moveAnimationDown.frames;
}

void Player::move(int x, int y) {
	int wall = (_tileSize - _phisTileSize) / 2;
	int pts2 = _phisTileSize / 2 + wall / 1.5;
	int pts2w = _tileSize / 2 + wall / 3;
	Rect phis = { _y - x*pts2, _x - y*pts2, _y + x*pts2, _x + y*pts2 };

	if(_map->isCanGoTo(phis, _x + x*pts2w, _y + y*pts2w)) {
		_x += x*_tileSize/12;
		_y += y*_tileSize/12;
		if(_map->isOutOfBounds(_x, _y)) _map->setBackToMap(_x, _y);
		Point pos = {_x - _tileSize / 2, _y - _tileSize / 2};
		_map->globalCoordinatesToScreen(pos, pos);
		_sprite->setPosition(pos.x, pos.y);

		if(x == -1) _sprite->replaceAnimation(_moveAnimationLeft);
		else if(x == 1) _sprite->replaceAnimation(_moveAnimationRight);
		else if(y == -1) _sprite->replaceAnimation(_moveAnimationUp);
		else if(y == 1) _sprite->replaceAnimation(_moveAnimationDown);
	}
}

void Player::draw() const {
	_sprite->draw();
}
