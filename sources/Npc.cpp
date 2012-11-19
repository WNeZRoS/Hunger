#include "Npc.h"

Npc::Npc(const Texture::Name texture, KillCallback killCallback, float speed) {
	_sprite = TileSprite::create(TextureAtlas::Loader(texture, 4, 4), 0, 0, 11);
	_map = NULL;
	_killCallback = killCallback;
	_lastPosition = 0.0f;
	_physSize = 0;
	_positionOffset = 0.0f;

	_speedMultip = speed >= 30 ? speed = 29 : (speed < 0 ? speed = 0 : speed);

	_lastMoveTime = getCurrentTime();
	_moveDirection = 0;

	for(int i = 0; i < 4; i++) {
		_moveAnimation[i].frames = NULL;
		_moveAnimation[i].framesCount = 0;
	}
}

Npc::~Npc() {
	delete _sprite;
}

bool Npc::moveInDirection(int x, int y) {
	if(_moveDirection == 0) _lastMoveTime = getCurrentTime();

	_stopPosition = Point::NaN;
	_moveDirection = Point_i(Float(x).sign(), Float(y).sign());
	return true;
}

bool Npc::moveTo(const Point& to) {
	if(!_map) return false;

	_stopPosition = to + _positionOffset;

	if(_moveDirection == 0) _lastMoveTime = getCurrentTime();

	Point dir = (_stopPosition - _position);

	if(dir.x.abs() > _map->getOne() * 5) dir.x *= -1;
	if(dir.y.abs() > _map->getOne() * 5) dir.y *= -1;
	_moveDirection = Point_i(dir.x.sign(), dir.y.sign());

	return false;
}

void Npc::stop() {
	_moveDirection = 0;
}

void Npc::draw() {
	_sprite->draw();
}

void Npc::onChangeWorld(const World *world) {
	Entity::onChangeWorld(world);

	_map = reinterpret_cast<LevelMap*>(world->getMap());
	if(!_map) return;

	_physSize = _map->getOne();
	_positionOffset = _physSize / 2.0f;
	_speed = _map->getOne() / (30 - _speedMultip);
	this->stop();
}

void Npc::onResize(const World *world) {
	Entity::onResize(world);
	_sprite->setScale(_map->getTileSize());
}

void Npc::onWorldScroll(const World *world) {
	Entity::onWorldScroll(world);
	this->updateSpritePosition();
}

void Npc::onOverlapBy(const Entity *overlap, const World *world) {
	Entity::onOverlapBy(overlap, world);
}

Entity::Category Npc::getCategory() const {
	return NPC;
}

int Npc::getPhysSize() const {
	return _physSize;
}

const Point& Npc::getPosition() const {
	return _position;
}

const Point& Npc::getLastPosition() const {
	return _lastPosition;
}

void Npc::setPosition(const Point &pos) {
	_lastPosition = _position;
	_position = pos;
	this->updateSpritePosition();
}

bool Npc::isOverlap(const Point& center, int radius) const {
	return this->isOverlap(center - radius, center + radius);
}

bool Npc::isOverlap(const Point& start, const Point& end) const {
	return start <= _position && _position <= end;
}

void Npc::updateSpritePosition() {
	if(!_map) return;

	Point position = _position - _positionOffset;
	_map->globalToScreen(position, position);
	_sprite->setPosition(position.x.f(), position.y.f());
}

Npc::MoveState Npc::move() {
	if(_moveDirection == 0) return Npc::NOT_MOVED;

	Timestamp current = getCurrentTime();
	if(_lastMoveTime + 50 > current) return Npc::NOT_MOVED;

	Timestamp deltaTime = current - _lastMoveTime;
	if(deltaTime > 300) deltaTime = 300;

	float multiply = deltaTime / 50.0f;
	_lastMoveTime = current;
	return move(multiply * _moveDirection.x, multiply * _moveDirection.y);
}

Npc::MoveState Npc::move(float x, float y) {
	Point position = getPosition();
	Point safePosition = position;
	Point wannaPosition = position + Point(x, y);

	if(!_stopPosition.nan()) {
		if(_stopPosition.y == position.y && _stopPosition.y == wannaPosition.y
				&& ((position.x < _stopPosition.x && _stopPosition.x < wannaPosition.x)
					|| (position.x > _stopPosition.x && _stopPosition.x > wannaPosition.x)))
			wannaPosition = _stopPosition;
		else if(_stopPosition.x == position.x && _stopPosition.x == wannaPosition.x
				&& ((position.y < _stopPosition.y && _stopPosition.y < wannaPosition.y)
					|| (position.y > _stopPosition.y && _stopPosition.y > wannaPosition.y)))
			wannaPosition = _stopPosition;
	}

	if(_map->getPathFinder()->correctMotion(position, wannaPosition)) {
		Point direction = position - safePosition;
		this->setPosition(position);

		if(direction.x > 0.01f) _sprite->replaceAnimation(_moveAnimation[RIGHT]);
		else if(direction.x < -0.01f) _sprite->replaceAnimation(_moveAnimation[LEFT]);
		else if(direction.y > 0.01f) _sprite->replaceAnimation(_moveAnimation[BOTTOM]);
		else if(direction.y < -0.01f) _sprite->replaceAnimation(_moveAnimation[TOP]);

		_world->updated(this);

		if(wannaPosition == _stopPosition) return Npc::MOVED_TO_STOP;

		return Npc::MOVED;
	}

	return Npc::NOT_MOVED;
}
