#include "Player.h"
#include "Api/Logger.h"

Player::Player(const Texture::Name texture) {
	_sprite = TileSprite::create(TextureAtlas::Loader(texture, 4, 4), 0, 0, 10);

	_speed = 3;

	_moveAnimationLeft.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationLeft.framesCount = 2;
	_moveAnimationLeft.loop = 1;
	_moveAnimationLeft.frames[0].tileId = 0; _moveAnimationLeft.frames[0].delay = 150;
	_moveAnimationLeft.frames[1].tileId = 4; _moveAnimationLeft.frames[1].delay = 150;

	_moveAnimationRight.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationRight.framesCount = 2;
	_moveAnimationRight.loop = 1;
	_moveAnimationRight.frames[0].tileId = 1; _moveAnimationRight.frames[0].delay = 150;
	_moveAnimationRight.frames[1].tileId = 5; _moveAnimationRight.frames[1].delay = 150;

	_moveAnimationUp.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationUp.framesCount = 2;
	_moveAnimationUp.loop = 1;
	_moveAnimationUp.frames[0].tileId = 2; _moveAnimationUp.frames[0].delay = 150;
	_moveAnimationUp.frames[1].tileId = 6; _moveAnimationUp.frames[1].delay = 150;

	_moveAnimationDown.frames = new TileSprite::Animation::Frame[2];
	_moveAnimationDown.framesCount = 2;
	_moveAnimationDown.loop = 1;
	_moveAnimationDown.frames[0].tileId = 3; _moveAnimationDown.frames[0].delay = 150;
	_moveAnimationDown.frames[1].tileId = 7; _moveAnimationDown.frames[1].delay = 150;
}

Player::~Player() {
	delete _sprite;
	delete [] _moveAnimationLeft.frames;
	delete [] _moveAnimationRight.frames;
	delete [] _moveAnimationUp.frames;
	delete [] _moveAnimationDown.frames;
}

void Player::onChangeWorld(const World *world) {
	_world = const_cast<World*>(world);
	_map = reinterpret_cast<LevelMap*>(world->getMap());
	_tileSize = _map->getTileSize();
	_position = _map->getPlayerSpawnPosition();
	_position += (_tileSize / 2.0f);

	Point pos = _position - (_tileSize / 2.0f);
	_map->globalCoordinatesToScreen(pos, pos);
	_sprite->setPosition(pos.x, pos.y);
	_sprite->setScale(_map->getTileSize());
}

void Player::onOverlapBy(const Entity *overlap, const World *world) {
	// TODO
}

const Entity::Category Player::getCategory() const {
	return PLAYER;
}

int Player::getPhysSize() const {
	return _tileSize;
}

bool Player::isOverlap(const Point &center, int radius) const {
	return false; // TODO
}

bool Player::isOverlap(const Point &start, const Point &end) const {
	return false; // TODO
}

bool Player::move(int x, int y) {
	Log::logger << Log::debug << "Pos: " << _position;
	Point dir( x, y );
	if(_map->moveInDirection(_position, dir, _speed)) {

		Point position = _position - (_tileSize / 2.0f);
		_map->globalCoordinatesToScreen(position, position);
		_sprite->setPosition(position.x, position.y);

		if(x == -1) _sprite->replaceAnimation(_moveAnimationLeft);
		else if(x == 1) _sprite->replaceAnimation(_moveAnimationRight);
		else if(y == -1) _sprite->replaceAnimation(_moveAnimationUp);
		else if(y == 1) _sprite->replaceAnimation(_moveAnimationDown);

		_world->updated(this);

		return true;
	}
	return false;

	//int wall = (_tileSize - _phisTileSize) / 2;
	//int pts2 = _phisTileSize / 2 + wall / 1.5;
	//int pts2w = _tileSize / 2 + wall / 3;

	//Point position = { _x, _y };
	//Point moveTo = { _x + x * _speed * _tileSize / 12, _y + y * _speed * _tileSize / 12 };

	/*if(_map->isCanGoTo(position, moveTo)) {
		//_x += x*_tileSize/12;
		//_y += y*_tileSize/12;
		_x = position.x;
		_y = position.y;

		if(_map->isOutOfBounds(_x, _y)) _map->setBackToMap(_x, _y);

		Point pos = {_x - _tileSize / 2, _y - _tileSize / 2};
		_map->globalCoordinatesToScreen(pos, pos);
		_sprite->setPosition(pos.x, pos.y);

		if(x == -1) _sprite->replaceAnimation(_moveAnimationLeft);
		else if(x == 1) _sprite->replaceAnimation(_moveAnimationRight);
		else if(y == -1) _sprite->replaceAnimation(_moveAnimationUp);
		else if(y == 1) _sprite->replaceAnimation(_moveAnimationDown);
	}*/
}

void Player::draw() const {
	_sprite->draw();
}
