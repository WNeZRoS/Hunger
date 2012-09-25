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

bool Monster::move() {
	return Npc::move();
}

bool Monster::move(float x, float y) {
	if(!Npc::move(x, y)) {
		this->stop();
		if(_intelligence) _intelligence->whatMeDo(this);
		return false;
	}
	return true;
}
