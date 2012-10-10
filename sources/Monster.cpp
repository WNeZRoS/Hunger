#include "Monster.h"
#include "Intelligence.h"

Monster::Monster(const Texture::Name texture, KillCallback killCallback, float speed,
				 Intelligence *intelligence) : Npc(texture, killCallback, speed) {
	_intelligence = intelligence;

	for(int i = 0; i < 4; i++) {
		_moveAnimation[i].frames = new TileSprite::Animation::Frame[2];
		_moveAnimation[i].framesCount = 2;
		_moveAnimation[i].loop = 1;
		_moveAnimation[i].frames[0].tileId = i;
		_moveAnimation[i].frames[0].delay = 150;
		_moveAnimation[i].frames[1].tileId = i + 4;
		_moveAnimation[i].frames[1].delay = 150;
	}
}

Monster::~Monster() {
	if(_intelligence) _intelligence->removeMonster(this);
}

void Monster::draw() {
	this->move();

	for(unsigned int i = 1; i < _path.size(); i++) {
		if(i == _step) Render::getPainter()->setColor(0x0, 0xFF, 0x0);
		else Render::getPainter()->setColor(0xFF, 0xFF, 0x0);

		Point pos, lpos;
		_map->globalToScreen(_path[i-1].pos + _positionOffset, lpos);
		_map->globalToScreen(_path[i].pos + _positionOffset, pos);
		Render::getPainter()->line(lpos.x.f(), lpos.y.f(), pos.x.f(), pos.y.f());
	}
	Render::getPainter()->setColor(0xFF, 0xFF, 0xFF);
	Npc::draw();
}

void Monster::onChangeWorld(const World *world) {
	Npc::onChangeWorld(world);

	if(!_map) return;

	_position = _map->getMobSpawnPosition() + _positionOffset;
	setPosition(_position);

	if(_intelligence) _intelligence->addMonster(this);
}

void Monster::onOverlapBy(const Entity *overlap, const World *world) {
	Log::Debug << "Monster overlap by " << overlap->getCategory();

	if(overlap->getCategory() == Entity::PLAYER) {
		Player *player = reinterpret_cast<Player*>(const_cast<Entity*>(overlap));
		if(player->isAngry()) {
			#warning TODO
		} else {
			player->onOverlapBy(this, world);
		}
	}
}

unsigned int Monster::getCurrentPathStep() const {
	for(unsigned int i = 0; i < _path.size(); i++) {
		if(_map->isOneTile(_path[i].pos, _position)) return i;
	}
	return -1;
}

bool Monster::moveByPath(const Array<PathFinder::PathElement> &path) {
	_path = path;
	unsigned int step = this->getCurrentPathStep();
	this->moveToStep(++step);
	return true;
}

void Monster::moveToStep(unsigned int step) {
	_step = step;
	this->moveTo(_path[_step].pos);
}

Npc::MoveState Monster::move() {
	return Npc::move();
}

Npc::MoveState Monster::move(float x, float y) {
	Npc::MoveState moveState = Npc::move(x, y);
	unsigned int step = this->getCurrentPathStep();

#warning TODO over move on turn to player
#warning FIX lag when move okolo quad

	if(moveState != Npc::MOVED) {
		if(step != _step && step + 1 != _path.size()) {
			this->moveToStep(++step);
		} else {
			this->stop();
			if(_intelligence) _intelligence->addEvent(Intelligence::WHAT_ME_DO, this);
		}
	} else if(step != _step) this->moveToStep(++step);
	return moveState;
}
