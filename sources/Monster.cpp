#include "Monster.h"
#include "Intelligence.h"
#include <cmath>

Monster::Monster(const Texture::Name texture, Intelligence *intelligence, float speed) {
	_sprite = TileSprite::create(TextureAtlas::Loader(texture, 4, 4), 0, 0, 11);
	_intelligence = intelligence;
	_map = NULL;
	_moveDirection = 0;
	_speedMultip = speed >= 30 ? speed = 29 : (speed < 0 ? speed = 0 : speed);
	_lastMoveTime = getCurrentTime();

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

Monster::~Monster() {
	delete _sprite;
}

void Monster::onChangeWorld(const World *world) {
	Entity::onChangeWorld(world);
	_map = reinterpret_cast<LevelMap*>(world->getMap());
	if(!_map) return;
	_position = _map->getMobSpawnPosition() + (_map->getOne() / 2.0f);
	setPosition(_position);
	_speed = _map->getOne() / (30 - _speedMultip);
	if(_map && _intelligence) _intelligence->whatMeDo(this);
}

void Monster::onResize(const World *world) {
	_sprite->setScale(_map->getTileSize());
}

void Monster::onWorldScroll(const World *world) {
	Point position = _position - (_map->getOne() / 2.0f);
	_map->globalCoordinatesToScreen(position, position);
	_sprite->setPosition(position.x, position.y);

	if(_map && _intelligence) _intelligence->whatMeDo(this);
}

void Monster::onOverlapBy(const Entity *overlap, const World *world) {
	Log::Debug << "Monster overlap by " << overlap->getCategory();
}

const Entity::Category Monster::getCategory() const {
	return NPC;
}

int Monster::getPhysSize() const {
	return _map ? _map->getOne() : 1;
}

bool Monster::isOverlap(const Point &center, int radius) const {
	return false; // TODO
}

bool Monster::isOverlap(const Point &start, const Point &end) const {
	return start <= _position && _position <= end;
}

void Monster::setPosition(const Point &pos) {
	_lastPosition = _position;
	_position = pos;

	if(!_map) return;
	Point screen = pos - (_map->getOne() / 2.0f);
	_map->globalCoordinatesToScreen(screen, screen);
	_sprite->setPosition(screen.x, screen.y);
}

void Monster::draw() {
	this->move();
	_sprite->draw();
}

void Monster::move() {
	Timestamp current = getCurrentTime();
	if(_lastMoveTime + 50 > current) return;
	if(_moveDirection == 0) return;
	if(current - _lastMoveTime < 400) {
		move((current - _lastMoveTime) / 50.0f * _moveDirection.x,
			 (current - _lastMoveTime) / 50.0f * _moveDirection.y);
	}
	_lastMoveTime = current;
}

bool Monster::move(float x, float y) {
	Point dir( x, y );
	Point position = _position;

	if(_map->moveInDirection(position, dir, _speed)) {
		//Log::Debug << "i can move to " << position;

		if((_moveDirection.y != 0 && ((_position.y < _stop.y && _stop.y < position.y)
									  || (_position.y > _stop.y && _stop.y > position.y)))) {
			position.y = _stop.y;
			this->stop();
		} else if((_moveDirection.x != 0 && ((_position.x < _stop.x && _stop.x < position.x)
											  || (_position.x > _stop.x && _stop.x > position.x)))) {
			position.x = _stop.x;
			this->stop();
		}

		dir = position - _position;

		if(dir.x > 100 || dir.x < -100) dir.x *= -1;
		if(dir.y > 100 || dir.y < -100) dir.y *= -1;

		//Log::Debug << "but moves to " << position;
		setPosition(position);

		if(dir.x < 0 && std::abs(dir.x) > std::abs(dir.y)) _sprite->replaceAnimation(_moveAnimationLeft);
		else if(dir.x > 0 && std::abs(dir.x) > std::abs(dir.y)) _sprite->replaceAnimation(_moveAnimationRight);
		else if(dir.y < 0) _sprite->replaceAnimation(_moveAnimationUp);
		else if(dir.y > 0) _sprite->replaceAnimation(_moveAnimationDown);

		_world->updated(this);

		if(_moveDirection == 0) {
			if(_intelligence) _intelligence->whatMeDo(this);
		}

		return true;
	} else {
		Log::Debug << "I can't move";
		this->stop();

		if(_moveDirection == 0) {
			if(_intelligence) _intelligence->whatMeDo(this);
		}
	}
	return false;
}

void Monster::stop() {
	_moveDirection = Point_i(0, 0);
	Log::Debug << "Moster stop";
}

bool Monster::moveTo(const Point& stop) {
	if(!_map) return false;
	_stop = stop + (_map->getOne() / 2.0f);

	_moveDirection = (_stop - _position).toPoint_i();
	if(std::fabs(_moveDirection.x) < std::fabs(_moveDirection.y)) _moveDirection.x = 0;
	else _moveDirection.y = 0;

	if(std::fabs(_moveDirection.x) > _map->getOne() * 2) _moveDirection.x *= -1;
	if(std::fabs(_moveDirection.y) > _map->getOne() * 2) _moveDirection.y *= -1;

	if(_moveDirection.x != 0) _moveDirection.x /= std::fabs(_moveDirection.x);
	if(_moveDirection.y != 0) _moveDirection.y /= std::fabs(_moveDirection.y);
	Log::Debug << "Monster must move to " << stop << " from " << _position << " in " << _moveDirection;
	return true;
}
