#include "Player.h"
#include <cmath>

Player::Player(const Texture::Name texture, KillCallback killCallback, float speed)
	: Npc(texture, killCallback, speed) {

	for(int i = 0; i < 4; i++) {
		_moveAnimation[i].frames = new TileSprite::Animation::Frame[2];
		_moveAnimation[i].framesCount = 2;
		_moveAnimation[i].loop = 1;
		_moveAnimation[i].frames[0].tileId = i;
		_moveAnimation[i].frames[0].delay = 150;
		_moveAnimation[i].frames[1].tileId = i + 4;
		_moveAnimation[i].frames[1].delay = 150;
	}

	_controller = new PlayerController(this);
}

Player::~Player() {
	delete _controller;
}

void Player::onChangeWorld(const World *world) {
	Npc::onChangeWorld(world);

	if(!_map) return;

	_position = _map->getPlayerSpawnPosition() + _positionOffset;
	this->setPosition(_position);
}

void Player::onResize(const World *world) {
	Npc::onResize(world);
	_world->setCenter(_position.x.f(), _position.y.f());
}

void Player::onOverlapBy(const Entity *overlap, const World *world) {
	Npc::onOverlapBy(overlap, world);

	Log::Debug << "Player overlap by " << overlap->getCategory();
	if(overlap->getCategory() == NPC) {
		if(_killCallback.method) (_killCallback.pointer->*(_killCallback.method))(this);
	}
}

Entity::Category Player::getCategory() const {
	return PLAYER;
}


void Player::draw() {
	Npc::move();
	Npc::draw();

	if(_angryTime > 0) {
		if(_angryTime < getCurrentTime()) {
			this->setAngry(0);
		}
	}
}

bool Player::move(float x, float y) {
	if(Npc::move(x, y)) {
		_world->setCenter(_position.x.f(), _position.y.f());
		return true;
	}

	return false;
}

void Player::setAngry(Timestamp angry) {
	_angryTime = getCurrentTime() + angry;
}

bool Player::isAngry() const {
	return _angryTime > 0;
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
