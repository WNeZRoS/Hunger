#include "Player.h"
#include <cmath>

Player::Player(const Texture::Name texture, KillCallback killCallback) {
	_sprite = TileSprite::create(TextureAtlas::Loader(texture, 4, 4), 0, 0, 10);
	_killCallback = killCallback;

	_speed = 1;
	_moveDirection = 0;
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

	_controller = new PlayerController(this);
}

Player::~Player() {
	delete _sprite;
	delete _controller;
}

void Player::onChangeWorld(const World *world) {
	Entity::onChangeWorld(world);
	_map = reinterpret_cast<LevelMap*>(world->getMap());
	if(!_map) return;
	_position = _map->getPlayerSpawnPosition() + (_map->getOne() / 2.0f);
	_lastPosition = _position;
	_speed = _map->getOne() / 15.0f;
}

void Player::onResize(const World *world) {
	_world->setCenter(_position.x, _position.y);
	_sprite->setScale(_map->getTileSize());
}

void Player::onWorldScroll(const World *world) {
	Point position = _position - (_map->getOne() / 2.0f);
	_map->globalCoordinatesToScreen(position, position);
	_sprite->setPosition(position.x, position.y);
}

void Player::onOverlapBy(const Entity *overlap, const World *world) {
	Log::Debug << "Player overlap by " << overlap->getCategory();
	if(overlap->getCategory() == NPC) {
		if(_killCallback.method) (_killCallback.pointer->*(_killCallback.method))();
	}
}

const Entity::Category Player::getCategory() const {
	return PLAYER;
}

int Player::getPhysSize() const {
	return _map ? _map->getOne() : 1;
}

bool Player::isOverlap(const Point &center, int radius) const {
	return false; // TODO
}

bool Player::isOverlap(const Point &start, const Point &end) const {
	return start <= _position && _position <= end;
}

bool Player::moveInDirection(int x, int y) {
	if(_moveDirection == 0) _lastMoveTime = getCurrentTime();
	_moveDirection = Point_i(x % 2, y % 2);
	return true;
}

void Player::stop() {
	_moveDirection = 0;
}

void Player::draw() {
	this->move();
	_sprite->draw();
}

void Player::move() {
	Timestamp current = getCurrentTime();
	if(_moveDirection == 0 || _lastMoveTime + 50 > current) return;

	move((current - _lastMoveTime) / 50.0f * _moveDirection.x,
		 (current - _lastMoveTime) / 50.0f * _moveDirection.y);
	_lastMoveTime = current;
}

bool Player::move(float x, float y) {
	Point dir( x, y );
	Point position = _position;
	//Log::logger << Log::debug << "Pos: " << _position << " Dir: " << dir << " " << x << " " << y;
	if(_map->moveInDirection(position, dir, _speed)) {
		dir = position - _position;
		if(dir.x > 100 || dir.x < -100) dir.x *= -1;
		if(dir.y > 100 || dir.y < -100) dir.y *= -1;

		_lastPosition = _position;
		_position = position;

		if(dir.x < 0 && std::abs(dir.x) > std::abs(dir.y)) _sprite->replaceAnimation(_moveAnimationLeft);
		else if(dir.x > 0  && std::abs(dir.x) > std::abs(dir.y)) _sprite->replaceAnimation(_moveAnimationRight);
		else if(dir.y < 0) _sprite->replaceAnimation(_moveAnimationUp);
		else if(dir.y > 0) _sprite->replaceAnimation(_moveAnimationDown);

		onWorldScroll(_world);
		_world->setCenter(_position.x, _position.y);
		_world->updated(this);

		return true;
	}
	return false;
}

bool Player::isAngry() const {
	return false;
}

Player::PlayerController::PlayerController(Player *player) {
	_player = player;
	_moveX = _moveY = 0;

	addControlEventForKey(Control::KEY_UP);
	addControlEventForKey(Control::KEY_DOWN);
	addControlEventForKey(Control::KEY_LEFT);
	addControlEventForKey(Control::KEY_RIGHT);
}

Player::PlayerController::~PlayerController() {
	Control::instance().removeEvent(this);
}

void Player::PlayerController::addControlEventForKey(Control::Keys key) {
	Control::instance().addEvent(Control::STATE_DOWN, key, this,
								 reinterpret_cast<Control::CallBackMethod>(&Player::PlayerController::keyDownEvent));
	Control::instance().addEvent(Control::STATE_UP, key, this,
								 reinterpret_cast<Control::CallBackMethod>(&Player::PlayerController::keyUpEvent));
}

void Player::PlayerController::keyDownEvent(const Control::Event &event, int x, int y) {
	switch(event.key) {
	case Control::KEY_UP: _moveY = -1; break;
	case Control::KEY_DOWN: _moveY = 1; break;
	case Control::KEY_LEFT: _moveX = -1; break;
	case Control::KEY_RIGHT: _moveX = 1; break;
	default: return;
	}

	_player->moveInDirection(_moveX, _moveY);
}

void Player::PlayerController::keyUpEvent(const Control::Event &event, int x, int y) {
	switch(event.key) {
	case Control::KEY_UP: if(_moveY == -1) _moveY = 0; break;
	case Control::KEY_DOWN: if(_moveY == 1) _moveY = 0; break;
	case Control::KEY_LEFT: if(_moveX == -1) _moveX = 0; break;
	case Control::KEY_RIGHT: if(_moveX == 1) _moveX = 0; break;
	default: return;
	}

	_player->moveInDirection(_moveX, _moveY);
}
