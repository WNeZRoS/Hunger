#include "World.h"

using namespace std;

World::World() {
	setMap(NULL);
}

World::~World() {
	if(_map) delete _map;
	for(vector<const Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
		delete *it;
}

void World::setMap(const Map *map) {
	_map = const_cast<Map*>(map);
}

Map * World::getMap() const {
	return _map;
}

int World::addEntity(const Entity *entity) {
	if(!entity) return -1;
	_entities.push_back(entity);
	return _entities.size() - 1;
}

void World::removeEntity(const Entity *entity) {
	for(vector<const Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
		if(*it == entity) _entities.erase(it, it);
}

const Entity * World::getEntity(int id) const {
	return _entities.at(id);
}

unsigned int World::getEntitiesCount() const {
	return _entities.size();
}

void World::setScreenSize(int width, int height) {
	_width = width;
	_height = height;

	if(_map) _map->fillScreen(width, height);
}

void World::draw() const {
	if(_map) _map->draw();

	for(vector<const Entity*>::const_iterator it = _entities.begin(); it != _entities.end(); it++) {
		const Entity* e = *it;
		e->draw();
	}
}
