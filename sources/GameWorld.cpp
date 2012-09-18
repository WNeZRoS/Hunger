#include "GameWorld.h"

GameWorld::GameWorld() {
	_firstUpdate = false;
}

GameWorld::~GameWorld() {

}

void GameWorld::restart() {
	_firstUpdate = false;
	if(_map) static_cast<LevelMap*>(_map)->closeGates();
	World::restart();
}

void GameWorld::updated(const Entity *upd) {
	if(!_firstUpdate && upd->getCategory() == Entity::PLAYER) {
		if(_map) {
			static_cast<LevelMap*>(_map)->openGates();
			_firstUpdate = true;
		}
	}
	World::updated(upd);
}
