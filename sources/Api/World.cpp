#include "World.h"

using namespace std;

World::World() {
	setMap(NULL);
}

World::~World() {
	if(_map) delete _map;
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
		delete *it;

	_entities.clear();
}

void World::setMap(const Map *map) {
	_map = const_cast<Map*>(map);
}

Map * World::getMap() const {
	return _map;
}

int World::addEntity(const Entity *entity) {
	if(!entity) return -1;
	_entities.push_back(const_cast<Entity*>(entity));
	const_cast<Entity*>(entity)->onChangeWorld(this);
	return _entities.size() - 1;
}

void World::removeEntity(Entity *entity) {
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
		if(*it == entity) {
			delete entity;
			_entities.erase(it);
			break;
		}
}

Entity * World::getEntity(int id) const {
	for(list<Entity*>::const_iterator it = _entities.begin(); it != _entities.end(); it++)
		if(--id == 0) return *it;
	return NULL;
}

unsigned int World::getEntitiesCount() const {
	return _entities.size();
}

void World::setScreenSize(int width, int height) {
	_width = width;
	_height = height;

	if(_map) _map->fillScreen(width, height);
}

void World::updated(const Entity *upd) {
	int size = upd->getPhysSize() / 4.0f;
	Point start = upd->getPosition() - size;
	Point end = upd->getPosition() + size;

	for(list<Entity*>::const_iterator it = _entities.begin(); it != _entities.end(); ) {
		Entity* e = *it++;
		if(e != upd && e->isOverlap(start, end))
			const_cast<Entity*>(e)->onOverlapBy(upd, this);
	}
}

void World::draw() {
	if(_map) _map->draw();

	for(int c = Entity::START; c != Entity::END; c++) {
		for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
			Entity* e = *it;
			if(e->getCategory() == c) e->draw();
		}
	}
}
