#include "World.h"
#include "compatibility.h"

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
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onChangeWorld(this);
		(*it)->onResize(this);
		(*it)->onWorldScroll(this);
	}
}

Map * World::getMap() const {
	return _map;
}

int World::addEntity(const Entity *entity) {
	if(!entity) return -1;
	_entities.push_back(const_cast<Entity*>(entity));
	const_cast<Entity*>(entity)->onChangeWorld(this);
	const_cast<Entity*>(entity)->onResize(this);
	const_cast<Entity*>(entity)->onWorldScroll(this);
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
	if(_width == width && _height == height) return;

	_width = width;
	_height = height;

	if(_map) _map->fillScreen(width, height);

	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onResize(this);
		(*it)->onWorldScroll(this);
	}
}

void World::setCenter(float x, float y) {
	if(!_map) return;

	float xOffset = _map->_xOffset, yOffset = _map->_yOffset;
	_map->setCenter(x, y);
	if(xOffset == _map->_xOffset && yOffset == _map->_yOffset) return;

	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onWorldScroll(this);
	}
}

void World::updated(const Entity *upd) {
	int size = upd->getPhysSize() / 4.0f;
	Point start = upd->getPosition() - size;
	Point end = upd->getPosition() + size;

	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); ) {
		Entity* e = *it++;
		if(e != upd && e->isOverlap(start, end))
			e->onOverlapBy(upd, this);
	}
}

void World::restart() {
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onChangeWorld(this);
		(*it)->onResize(this);
		(*it)->onWorldScroll(this);
	}
}

void World::draw() {
	if(_map) _map->draw();

	for(int c = Entity::START; c != Entity::END; c++) {
		for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
			if((*it)->getCategory() == c) (*it)->draw();
		}
	}
}
